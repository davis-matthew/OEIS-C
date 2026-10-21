/*
 * A073127  a(n) is the quotient arising in A073125 [or in A073126]:
 *          a(n)=[A073126(n)-A073125(n)]/n=[p(s+n)-p(s)]/n.
 * https://oeis.org/A073127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = (p(s+n) – p(s)) / n * where s is the least positive
 *            integer such that (p(s+n) – p(s)) is divisible by n. * p(k)
 *            denotes the k‑th prime (p(1)=2)
 * Range:     u8 n <= 13, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* ---------- prime storage --------------------------------------- */
static seq_t *primes = NULL;         /* array of primes (0‑based: primes[0] = 2) */
static size_t primes_len = 0;        /* number of primes currently stored   */
static size_t primes_cap = 0;        /* allocated capacity of the array      */
static seq_t prime_last_checked = 1; /* last integer examined for primeness */

/* enlarge the prime array until it contains at least `needed_index` primes */
static void ensure_primes_at_least_index(idx_t needed_index)
{
    if (primes_len >= needed_index)
        return;

    /* allocate more space if necessary (never shrink) */
    if (primes_cap < (size_t)needed_index) {
        seq_t *tmp = (seq_t *)realloc(primes, (size_t)needed_index * sizeof(seq_t));
        if (!tmp)
            abort();
        primes = tmp;
        primes_cap = (size_t)needed_index;
    }

    /* test successive integers for primality by trial division */
    seq_t cand = prime_last_checked + (seq_t)1;
    while (primes_len < (size_t)needed_index) {
        bool is_p = true;
        seq_t root = floor_sqrt(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= root; ++i) {
            if (cand % primes[i] == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p)
            primes[primes_len++] = cand;
        ++cand;
    }
    prime_last_checked = cand - (seq_t)1;
}

/* ---------- sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; guard against division by zero */
        return 0;

    idx_t s = 1;         /* start searching for smallest s ≥ 1 */
    while (true) {
        ensure_primes_at_least_index(s + n);         /* need primes[0..s+n-1] */
        seq_t diff = primes[s + n - 1] - primes[s - 1];
        if ((diff % (seq_t)n) == 0)
            return diff / (seq_t)n;
        ++s;
    }
}
