/*
 * A127641  A127640 * A051731 as infinite lower triangular matrices.
 * https://oeis.org/A127641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 0 if the column index does not divide the row number;
 *            a(n) = p(row) otherwise, where p(k) is the k‑th prime and row,
 *            column are obtained from the triangular indexing of the
 *            lower‑triangular matrix
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is small – the maximum row needed for n ≤ 10000 is 141 – */
/* A trivial prime sieve stored in static arrays is therefore more than */
/* sufficient, and keeps the code portable to both C and C++.          */

#include <stddef.h>
#include <stdint.h>   /* for uint64_t */

/* The build supplies seq_t and idx_t.  We use them throughout. */

static seq_t primes_seq[2000];        /* p(k) as seq_t, 0‑based */
static uint64_t primes_uint64[2000]; /* same primes as uint64_t for tests */
static idx_t prime_cnt = 0;           /* number of primes known */

/* Ensure that at least 'need' primes are stored in the static arrays. */
static void
ensure_primes(idx_t need)
{
    if (prime_cnt >= need) return;

    uint64_t candidate = (prime_cnt == 0) ? 2 : primes_uint64[prime_cnt - 1] + 1;

    while (prime_cnt < need) {
        /* Find the next prime after 'candidate'. */
        while (1) {
            int is_prime = 1;
            for (idx_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = primes_uint64[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) { is_prime = 0; break; }
            }
            if (is_prime) break;
            ++candidate;
        }

        /* Store the new prime. */
        if (prime_cnt < 2000) {    /* defensive, should not trigger for n≤10000 */
            primes_uint64[prime_cnt] = candidate;
            primes_seq[prime_cnt]    = (seq_t)candidate;
            ++prime_cnt;
        }
        /* Prepare the next candidate. */
        candidate = primes_uint64[prime_cnt - 1] + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* behaviour below offset is irrelevant */

    /* Find the row r such that T(r-1) < n ≤ T(r) where T(k)=k(k+1)/2. */
    idx_t row = 1;
    idx_t t   = 1;           /* T(row) */
    while (t < n) {
        ++row;
        t += row;            /* now t = T(row) */
    }

    /* Column index (1‑based).  T(row-1) = t - row. */
    idx_t col = n - (t - row);

    /* Non‑zero iff column divides row. */
    if (row % col != 0) return (seq_t)0;

    /* Return the row‑th prime. */
    ensure_primes(row);
    return primes_seq[row - 1];
}
