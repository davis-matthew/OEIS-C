/*
 * A105048  Numbers n such that prime(n+2) - ( prime(n+1)-prime(n) ) is a
 *          semiprime.
 * https://oeis.org/A105048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth integer for which prime(n+2) -
 *            (prime(n+1)-prime(n)) is a semiprime. Here prime(k) denotes the
 *            k‑th prime. The function scans integers starting at 1, evaluates
 *            this expression for each, and records those that give a
 *            semiprime
 * Range:     u8 n <= 36, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------ dynamic arrays utilities ------------- */
static void ensure_cap(idx_t *cap, seq_t **arr, idx_t need)
{
    if (*cap < need) {
        idx_t newcap = (*cap == 0) ? 8 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        *arr = (seq_t*)realloc(*arr,
                               (size_t)newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* ------------ prime table ------------- */
static seq_t *prime_tbl = NULL;   /* prime_tbl[0] = 2  */
static idx_t  prime_cnt = 0;
static idx_t  prime_cap = 0;

/* test whether u is prime using previously found primes */
static bool is_prime_u64(uint64_t u)
{
    if (u < 2) return false;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)prime_tbl[i];
        if (p*p > u) break;
        if (u % p == 0) return false;
    }
    return true;
}

static void generate_next_prime(void)
/* append next prime to prime_tbl (1‑based indexing) */
{
    uint64_t cand;
    if (prime_cnt == 0) cand = 2;
    else {
        cand = (uint64_t)prime_tbl[prime_cnt-1] + 1;
        if ((cand & 1) == 0) cand++;          /* now odd */
    }
    while (true) {
        if (is_prime_u64(cand)) break;
        cand += 2;                            /* skip even numbers */
    }
    ensure_cap(&prime_cap, &prime_tbl, prime_cnt + 1);
    prime_tbl[prime_cnt] = (seq_t)cand;
    ++prime_cnt;
}

/* return the k‑th prime (k >= 1) */
static seq_t get_prime(idx_t k)
{
    while (prime_cnt < k) generate_next_prime();
    return prime_tbl[k-1];
}

/* ------------ semiprime test ------------- */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;   /* smallest semiprime is 4 = 2*2 */
    uint64_t rem = x;
    int factor_cnt = 0;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = (uint64_t)prime_tbl[i];
        if (p * p > rem) break;
        while (rem % p == 0) {
            ++factor_cnt;
            rem /= p;
            if (factor_cnt > 2) return false;
        }
    }
    if (rem > 1) ++factor_cnt;
    return factor_cnt == 2;
}

/* ------------ sequence storage ------------- */
static seq_t *term_tbl = NULL;
static idx_t  term_cnt = 0;
static idx_t  term_cap = 0;
static idx_t  last_checked = 0;   /* last integer i already examined */

/* Return a(n) (a(1) is first term) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset 1 – n>=1 in practice */
    /* compute terms up to n */
    while (term_cnt < n) {
        ++last_checked;          /* candidate integer i */
        /* ensure we have primes up to i+2 */
        seq_t p_i   = get_prime(last_checked);
        seq_t p_ip1 = get_prime(last_checked + 1);
        seq_t p_ip2 = get_prime(last_checked + 2);
        /* compute expression: p_{i+2} - (p_{i+1} - p_i) */
        seq_t diff = p_ip1 - p_i;
        seq_t val  = p_ip2 - diff;
        if (is_semiprime_uint64((uint64_t)val)) {
            ensure_cap(&term_cap, &term_tbl, term_cnt + 1);
            term_tbl[term_cnt] = (seq_t)last_checked;
            ++term_cnt;
        }
    }
    return term_tbl[n-1];
}
