/*
 * A218852  Numbers n for which sigma(n) = sigma(x) + sigma(y) + sigma(z),
 *          where n = x + y + z, with x, y, z all positive.
 * https://oeis.org/A218852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: a(n) = n‑th integer m such that
 *   sigma(m) = sigma(x) + sigma(y) + sigma(z)
 * with   m = x + y + z  and  x, y, z > 0.
 * 
 * The implementation builds the sequence lazily: a σ‑sieve of divisor
 * sums is maintained, and as larger indices are requested we test
 * successive integers until the required amount of terms has been
 * discovered.  The procedure is fast enough for the first ten thousand
 * entries (the 10 000th term comfortably fits within a few tens of
 * thousands).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------- internal state ------- */
static uint64_t *sigma = NULL;   /* divisor sums */
static size_t sigma_limit = 0;   /* current size of sigma */

static seq_t  *seq_val   = NULL;  /* cached sequence terms */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* allocated capacity of seq_val */

static idx_t  last_tested = 2;    /* highest integer already examined */

/* ------- helper: linear σ‑sieve up to “limit” ------- */
static void recompute_sigma(size_t limit)
{
    uint64_t *new_sigma = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    if (!new_sigma) return;          /* allocation failed – safety net */

    for (size_t i = 0; i <= limit; ++i) new_sigma[i] = 0;

    for (size_t i = 1; i <= limit; ++i) {
        for (size_t j = i; j <= limit; j += i) {
            new_sigma[j] += i;
        }
    }

    free(sigma);
    sigma = new_sigma;
    sigma_limit = limit;
}

/* ------- ensure we have at least the first n terms ------- */
static void ensure_terms(idx_t n)
{
    /* Be conservative with the σ‑sieve size */
    if (sigma == NULL) recompute_sigma(20000);

    while (seq_len < (size_t)n) {
        /* examine next candidate */
        ++last_tested;
        if ((size_t)last_tested > sigma_limit) {
            /* enlarge sieve */
            recompute_sigma(last_tested * 2);
        }

        if (last_tested < 3) continue;            /* need at least 3 parts */

        uint64_t sigma_n = sigma[(size_t)last_tested];
        bool ok = false;

        for (idx_t x = 1; x <= last_tested - 2 && !ok; ++x) {
            uint64_t sx = sigma[(size_t)x];
            if (sx > sigma_n) continue;          /* target would be negative */

            idx_t rem = last_tested - x;
            if (rem < 2) continue;               /* need y + z >= 2 */

            uint64_t target = sigma_n - sx;

            for (idx_t y = 1; y <= rem - 1 && !ok; ++y) {
                idx_t z = rem - y;
                if (sigma[(size_t)y] + sigma[(size_t)z] == target) {
                    ok = true;
                    break;
                }
            }
        }

        if (ok) {
            /* store term, grow array if necessary */
            if (seq_len + 1 > seq_cap) {
                size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
                seq_t *tmp = (seq_t*)realloc(seq_val, new_cap * sizeof(seq_t));
                if (!tmp) return;               /* allocation failed */
                seq_val = tmp;
                seq_cap = new_cap;
            }
            seq_val[seq_len++] = (seq_t)last_tested;
        }
    }
}

/* ------- public function required by the harness ------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined index */

    ensure_terms(n);                           /* compute up to index n */
    return seq_val[n - 1];                     /* zero‑based array */
}
