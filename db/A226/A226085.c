/*
 * A226085  Irregular triangular array T(n,k) of consecutive composites.
 * https://oeis.org/A226085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Π_{i=1}^{k} p_i + 2 + (n - Σ_{j=1}^{k-1} (p_{j+1}-2) - 1)
 *            where k is the unique integer with Σ_{i=1}^{k} (p_{i+1}-2) ≥ n
 * Range:     u8 n <= 18, u16 n <= 44, u32 n <= 109, u64 n <= 349,
 *            u128 n <= 1210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- prime storage ---- */
static uint64_t *prime_list   = NULL;   /* prime_list[0] = p1 = 2   */
static size_t   prime_cnt     = 0;      /* number of primes stored  */
static size_t   prime_cap     = 0;

/* ---- cumulative row length storage ---- */
static idx_t   *cumlen_list   = NULL;   /* cumlen_list[i] = Σ_{j=1}^{i+1} (p_{j+1}-2)  */
static size_t   cumlen_cnt    = 0;      /* number of rows stored */
static size_t   cumlen_cap    = 0;

/* Grow prime array */
static void grow_prime_cap(void)
{
    if (prime_cap == 0) prime_cap = 16;
    else                prime_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    prime_list = tmp;
}

/* Grow cumlen array */
static void grow_cumlen_cap(void)
{
    if (cumlen_cap == 0) cumlen_cap = 16;
    else                cumlen_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(cumlen_list, cumlen_cap * sizeof(idx_t));
    cumlen_list = tmp;
}

/* Generate next prime and store it */
static void generate_next_prime(void)
{
    uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt-1] + 1;
    if (candidate <= 2) candidate = 2;
    if (candidate == 2) {
        if (prime_cnt == prime_cap) grow_prime_cap();
        prime_list[prime_cnt++] = 2;
        return;
    }
    if ((candidate & 1) == 0) candidate++;   /* make odd */
    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_cnt == prime_cap) grow_prime_cap();
            prime_list[prime_cnt++] = candidate;
            return;
        }
        candidate += 2;   /* skip even numbers */
    }
}

/* Ensure at least 'count' primes are stored (p_1 … p_count) */
static void ensure_primes(size_t count)
{
    while (prime_cnt < count) generate_next_prime();
}

/* Add next row to cumlen_list if not already present */
static void add_row(void)
{
    size_t k = cumlen_cnt + 1;         /* next row number (1‑based) */
    /* need primes up to p_{k+1} for length, up to p_k for product */
    ensure_primes(k + 1);

    idx_t len = prime_list[k] - 2;     /* length of this row   */
    idx_t cum = (cumlen_cnt == 0) ? len : cumlen_list[cumlen_cnt-1] + len;

    if (cumlen_cnt == cumlen_cap) grow_cumlen_cap();
    cumlen_list[cumlen_cnt++] = cum;
}

/* Build rows until we have at least 'n' terms */
static void build_up_to(idx_t n)
{
    while (cumlen_cnt == 0 || cumlen_list[cumlen_cnt-1] < n) add_row();
}

/* Binary search for the row containing the n-th term */
static size_t find_row(idx_t n)
{
    size_t lo = 0, hi = cumlen_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (n <= cumlen_list[mid]) hi = mid;
        else                       lo = mid + 1;
    }
    return lo + 1;   /* return 1‑based row number */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* undefined for n = 0, but guard against accidental call */
    if (n == 0) return (seq_t)0;

    build_up_to(n);
    size_t k = find_row(n);          /* 1‑based row number */
    idx_t prev_cum = (k == 1) ? 0 : cumlen_list[k-2];
    idx_t offset  = n - prev_cum - 1;  /* zero‑based offset within the row */

    /* compute product of the first k primes */
    seq_t prod = 1;
    for (size_t i = 0; i < k; ++i) {
        prod *= (seq_t)prime_list[i];
    }

    return prod + (seq_t)2 + (seq_t)offset;
}
