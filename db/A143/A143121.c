/*
 * A143121  Triangle read by rows, T(n,k) = Sum_{j=k..n} prime(j), 1 <= k <=
 *          n.
 * https://oeis.org/A143121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = sum_{j=k..m} prime(j) where (m,k) are the row and column
 *            indices obtained by reading the triangle T(m,k) = Σ_{j=k..m}
 *            prime(j) row by row, starting with a(1) = prime(1)
 * Range:     u8 n <= 91, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Prime generator: stores the first primes in a dynamically grown   */
/*  array 'prime_tbl' (0‑based, prime_tbl[0] = 2).                      */
/* --------------------------------------------------------------------- */
static uint64_t *prime_tbl   = NULL;   /* array of primes */
static size_t   prime_cnt   = 0;      /* number of primes stored   */
static size_t   prime_cap   = 0;      /* allocated capacity        */
static uint64_t prime_next  = 2;      /* next candidate to test   */

/* Ensure that at least 'need' primes are in prime_tbl.             */
static void ensure_primes(idx_t need)
{
    if (prime_cnt >= need) return;

    /* Allocate or grow storage.                                */
    if (prime_tbl == NULL) {
        prime_cap = (size_t) (need > 16 ? need : 16);
        prime_tbl = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_tbl) abort();
    } else if (prime_cnt >= prime_cap) {
        prime_cap *= 2;
        void *tmp = realloc(prime_tbl, prime_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_tbl = (uint64_t *)tmp;
    }

    /* Generate primes until we have 'need' of them.           */
    while (prime_cnt < need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt &&
             prime_tbl[i] * prime_tbl[i] <= prime_next; ++i) {
            if (prime_next % prime_tbl[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt >= prime_cap) {
                prime_cap *= 2;
                void *tmp = realloc(prime_tbl, prime_cap * sizeof(uint64_t));
                if (!tmp) abort();
                prime_tbl = (uint64_t *)tmp;
            }
            prime_tbl[prime_cnt++] = prime_next;
        }
        ++prime_next;
    }
}

/* --------------------------------------------------------------------- */
/*  OEIS A143121: triangular sums of primes.                            */
/*  a(n) enumerates the triangle read by rows.                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* offset is 1 */

    /* Find the row r such that r(r+1)/2 >= n */
    idx_t low  = 1, high = 2;
    while (high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n) high = mid;
        else                          low = mid + 1;
    }
    idx_t r = low;                           /* row number (1‑based) */

    /* Column index (1‑based) */
    idx_t prev_tri = r * (r - 1) / 2;   /* triangular number of previous row */
    idx_t k = n - prev_tri;              /* column index */

    /* Ensure we have first r primes ready */
    ensure_primes((idx_t)r);

    /* Sum primes from k to r inclusive (k and r are 1‑based) */
    seq_t sum = (seq_t)0;
    for (idx_t i = k - 1; i < r; ++i) {
        sum += (seq_t)prime_tbl[i];
    }
    return sum;
}
