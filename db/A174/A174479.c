/*
 * A174479  Triangle read by rows: T(n,k) = semiprime(n) mod A065516(k), where
 *          A065516 are differences between consecutive semiprimes.
 * https://oeis.org/A174479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle: T(n,k) = semiprime(n) mod (semiprime(k+1)-semiprime(k)).
   a(n) is the n‑th term of the triangle read row‑by‑row. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"

static seq_t *semiprimes = NULL;
static size_t semiprime_cap = 0;
static size_t semiprime_cnt = 0;
static uint64_t semiprime_candidate = 4;

static void ensure_semiprimes(idx_t needed)
{
    while (semiprime_cnt < needed) {
        if (semiprime_cnt == semiprime_cap) {
            size_t new_cap = semiprime_cap == 0 ? 16 : semiprime_cap * 2;
            seq_t *tmp = (seq_t *)realloc(semiprimes, new_cap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            semiprimes = tmp;
            semiprime_cap = new_cap;
        }
        if (!is_prime(semiprime_candidate)) {
            uint64_t p = _smallest_prime_factor(semiprime_candidate);
            if (p != 0) {
                uint64_t q = semiprime_candidate / p;
                if (q == p || is_prime(q)) {
                    semiprimes[semiprime_cnt++] = (seq_t)semiprime_candidate;
                }
            }
        }
        ++semiprime_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, offset starts at 1 */

    /* find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = 1;
    while (tri < n) {
        ++r;
        tri += r;
    }
    /* column in that row */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t k = n - prev_tri;            /* 1 <= k <= r */

    /* we need semiprimes up to index r+1 (to have the needed difference) */
    ensure_semiprimes((idx_t)(r + 1));

    seq_t sp_r = semiprimes[(size_t)(r - 1)];
    seq_t diff_k = semiprimes[(size_t)k] - semiprimes[(size_t)(k - 1)];
    return sp_r % diff_k;
}
