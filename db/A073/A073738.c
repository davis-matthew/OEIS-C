/*
 * A073738  Sum of every other prime <= n-th prime down to 2 or 1; equals the
 *          partial sums of A036467 (in which sums of two consecutive terms
 *          form the primes).
 * https://oeis.org/A073738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = prime(n) + a(n-2) with a(0)=1, a(1)=2
 * Range:     u8 n <= 17, u16 n <= 214, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- Prime table ----------------------------------------------------- */

static idx_t      *prime_tbl = NULL;   /* primes[0]=2, primes[1]=3, ...   */
static size_t      prime_cnt  = 0;      /* number of stored primes          */
static size_t      prime_cap  = 0;      /* allocated capacity               */
static idx_t       last_checked = 2;   /* next candidate after 2           */

/* Ensure that prime_tbl contains at least indices 0 … limit_idx. */
static void
ensure_primes_upto_idx (idx_t limit_idx)
{
    if (prime_cnt > limit_idx)
        return;

    while (prime_cnt <= limit_idx) {
        idx_t cand  = last_checked;
        int   is_p  = 1;

        /* trial division by known primes up to sqrt(cand) */
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_tbl[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_p = 0;
                break;
            }
        }

        if (is_p) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 64;
                idx_t  *tmp = (idx_t *)realloc (prime_tbl, new_cap * sizeof (idx_t));
                if (!tmp) abort ();
                prime_tbl = tmp;
                prime_cap = new_cap;
            }
            prime_tbl[prime_cnt++] = cand;
        }

        if (cand == 2)
            last_checked = 3;
        else
            last_checked += 2;
    }
}

/* Return the m‑th prime in OEIS sense (prime(0)=1). */
static seq_t
get_prime (idx_t m)
{
    if (m == 0)
        return (seq_t)1;   /* special definition for prime(0) */
    ensure_primes_upto_idx (m - 1);
    return (seq_t)prime_tbl[m - 1];
}

/* --- Sequence table --------------------------------------------------- */

static seq_t  *seq_tbl = NULL;   /* seq_tbl[i] = a(i)                  */
static size_t  seq_cap = 0;      /* allocated capacity                 */
static size_t  seq_cnt = 0;      /* number of computed indices         */

/* Make seq_tbl large enough to store indices 0 … new_cap-1. */
static void
ensure_seq_cap (size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;

    size_t cap = seq_cap ? seq_cap * 2 : 64;
    while (cap < new_cap) cap <<= 1;

    seq_t *tmp = (seq_t *)realloc (seq_tbl, cap * sizeof (seq_t));
    if (!tmp) abort ();
    seq_tbl   = tmp;
    seq_cap   = cap;
}

/* Compute a(n) using the recurrence a(n)=prime(n)+a(n-2). */
seq_t
a (idx_t n)
{
    if (n < seq_cnt)
        return seq_tbl[n];

    /* Ensure space for all indices up to n. */
    ensure_seq_cap (n + 1);

    /* Initialise base cases if not already done. */
    if (seq_cnt == 0) {
        seq_tbl[0] = 1;
        seq_cnt   = 1;
    }
    if (seq_cnt == 1 && n >= 1) {
        seq_tbl[1] = 2;
        seq_cnt   = 2;
    }

    /* Compute missing values. */
    for (size_t i = seq_cnt; i <= n; ++i) {
        seq_tbl[i] = seq_tbl[i - 2] + get_prime (i);
    }
    seq_cnt = n + 1;

    return seq_tbl[n];
}
