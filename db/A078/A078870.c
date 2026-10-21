/*
 * A078870  Decimal concatenations of the 38 quintuples (d1,d2,d3,d4,d5) with
 *          elements in {2,4,6} for which there exists a prime p >= 7 such
 *          that the differences between the 6 consecutive primes starting
 *          with p are (d1,d2,d3,d4,d5).
 * https://oeis.org/A078870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, base, nonn
 * Formula:   a(n) is the nth smallest decimal concatenation of a 5‑tuple
 *            (d1,d2,d3,d4,d5) with d1…d5∈{2,4,6} such that there exists a
 *            prime p≥7 with consecutive prime gaps (p, p+d1, …, p+d1+…+d5)
 *            being exactly (d1,d2,d3,d4,d5). If n is larger than the number
 *            of such tuples, a(n) = 0
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   cmp_seqt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/cmp_seqt.h"

/* maximum number of candidate patterns that can occur (3^5 = 243) */
#define MAX_PATTERNS 243

static int   patterns_cnt = 0;          /* number of distinct patterns found   */
static seq_t patterns[MAX_PATTERNS];     /* the unique concatenated numbers     */
static bool  patterns_inited = false;   /* guard for one‑time initialization   */

/* initialise the table of patterns; executed on first call to a() */
static void init_patterns(void)
{
    /* limit chosen large enough that all 38 patterns are seen.
       20 000 000 requires about 20 MB of memory for the sieve.      */
    const size_t SIEVE_LIMIT = 20000000U;

    /* sieve */
    bool *is_prime = (bool *)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) { patterns_inited = true; return; }          /* malloc fail */

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    /* ring buffer for the last six primes */
    unsigned sixth[6];
    int use = 0;

    for (size_t num = 2; num <= SIEVE_LIMIT; ++num) {
        if (!is_prime[num]) continue;

        if (use < 6) {
            sixth[use++] = (unsigned)num;
            continue;
        }

        /* we now have six consecutive primes in `sixth[0..5]` */
        if (sixth[0] >= 7) {
            bool ok = true;
            unsigned digits = 0;

            for (int k = 0; k < 5 && ok; ++k) {
                unsigned diff = sixth[k + 1] - sixth[k];
                if (diff != 2 && diff != 4 && diff != 6) ok = false;
                digits = digits * 10 + diff;   /* digit d1…d5 */
            }

            if (ok) {
                /* store unique pattern */
                bool present = false;
                for (int i = 0; i < patterns_cnt; ++i)
                    if (patterns[i] == (seq_t)digits) { present = true; break; }

                if (!present) {
                    patterns[patterns_cnt++] = (seq_t)digits;
                    if (patterns_cnt >= 38) break;   /* all patterns found */
                }
            }
        }

        /* slide the ring buffer left by one */
        for (int k = 0; k < 5; ++k) sixth[k] = sixth[k + 1];
        sixth[5] = (unsigned)num;
    }

    free(is_prime);

    /* sort the unique patterns so that indexing is correct */
    qsort(patterns, patterns_cnt, sizeof(seq_t), cmp_seqt);

    patterns_inited = true;
}

/* exported function:  seq_t a(idx_t n)  */
seq_t a(idx_t n)
{
    if (!patterns_inited) init_patterns();

    if (n == 0) return 0;                     /* n<1 is undefined; return 0 */
    if (n > (idx_t)patterns_cnt) return 0;    /* beyond the list */

    return patterns[(size_t)(n - 1)];
}
