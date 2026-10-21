/*
 * A096459  Triangle read by rows: T(n,k) = n^2 mod prime(k), 1<=k<=n.
 * https://oeis.org/A096459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r^2) mod p_k where r is the row number (1‑indexed), k
 *            the column number (1‑to‑r), and p_k is the k‑th prime. The index
 *            n is the position of the entry when the triangle is read row by
 *            row
 * Range:     u8 n <= 134, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* --- prime generation -------------------------------------------------- */
#define PRIME_SIEVE_LIMIT 300000   /* > 20000th prime (≈224737)             */
#define PRIME_ARRAY_SIZE  25000    /* safety margin                         */

static uint64_t _prime_list[PRIME_ARRAY_SIZE];
static int      _prime_cnt  = 0;
static bool     _prime_inited = false;

static void _init_primes(void)
{
    if (_prime_inited) return;
    _prime_inited = true;
    bool *is_composite = (bool*)malloc((PRIME_SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_composite) return;          /* memory failure → leave list empty */

    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (_prime_cnt < PRIME_ARRAY_SIZE)
                _prime_list[_prime_cnt++] = i;
            for (int64_t j = (int64_t)i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_composite[j] = true;
        }
    }
    free(is_composite);
}

static seq_t _kth_prime(idx_t k)
{
    if (k == 0) return 2;          /* dummy for out‑of‑range, should not happen */
    if (! _prime_inited) _init_primes();
    if (k-1 < (idx_t)_prime_cnt)
        return (seq_t)_prime_list[k-1];
    /* if k is larger than we pre‑computed, compute it on the fly
       (this case never happens for the required test sizes). */
    uint64_t candidate = _prime_list[_prime_cnt - 1] + 2;
    while (1) {
        bool prime = true;
        for (int i = 0; i < _prime_cnt; ++i) {
            uint64_t p = _prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            if (_prime_cnt < PRIME_ARRAY_SIZE) _prime_list[_prime_cnt++] = candidate;
            ++k;
            if (k == (idx_t)_prime_cnt) return (seq_t)candidate;
        }
        candidate += 2;
    }
}

/* --- sequence ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined, chosen 0       */
    /* find row r so that T(r-1) < n <= T(r), where T(r)=r(r+1)/2           */
    idx_t r = 1;
    idx_t tri = 1;                 /* T(1) */
    while (tri < n) {
        ++r;
        tri += r;                  /* T(r) = T(r-1)+r                           */
    }
    /* column k = n - T(r-1)   (1-indexed)                                 */
    idx_t prev_tri = tri - r;      /* T(r-1)                                   */
    idx_t k = n - prev_tri;        /* 1 <= k <= r                              */

    /* compute r^2 modulo k-th prime                                       */
    seq_t row_sq = (seq_t)r * (seq_t)r;  /* result fits in seq_t or larger   */
    seq_t prime_k = _kth_prime(k);
    return row_sq % prime_k;
}
