/*
 * A232282  Number of 2 X n 0..2 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A232282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -23*a(n-2) +20*a(n-3) +81*a(n-4) * -511*a(n-5)
 *            +294*a(n-6) +150*a(n-7) -168*a(n-8) * +1692*a(n-9) +432*a(n-10)
 *            * with initial values (n = 1..10): * a(1)=9, a(2)=35, a(3)=199,
 *            a(4)=1066, a(5)=6019, * a(6)=32301, a(7)=174400, a(8)=944500,
 *            a(9)=5142177, * a(10)=27937799
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] is dummy, cache[1] holds a(1) */
    static idx_t cache_size = 0;     /* number of elements currently cached */

    /* initial terms from the OEIS entry */
    static const seq_t base[10] = {
        (seq_t)9ULL,
        (seq_t)35ULL,
        (seq_t)199ULL,
        (seq_t)1066ULL,
        (seq_t)6019ULL,
        (seq_t)32301ULL,
        (seq_t)174400ULL,
        (seq_t)944500ULL,
        (seq_t)5142177ULL,
        (seq_t)27937799ULL
    };

    if (n == 0) return 0;            /* undefined, keep safe */

    /* Resize cache if necessary */
    if (cache_size <= n) {
        size_t new_len = (size_t)(n + 1);   /* +1 because index n is used */
        seq_t *new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_cache) abort();           /* out of memory */

        /* copy existing values */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        } else {
            memset(new_cache, 0, cache_size * sizeof(seq_t));
        }

        /* Fill newly required entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                new_cache[i] = 0;
                continue;
            }
            if (i <= 10) {             /* base cases */
                new_cache[i] = base[i - 1];
            } else {                   /* recurrence of order 10 */
                seq_t pos = 0;
                seq_t neg = 0;

                pos += new_cache[i - 1] * (seq_t)9;
                pos += new_cache[i - 3] * (seq_t)20;
                pos += new_cache[i - 4] * (seq_t)81;
                pos += new_cache[i - 6] * (seq_t)294;
                pos += new_cache[i - 7] * (seq_t)150;
                pos += new_cache[i - 9] * (seq_t)1692;
                pos += new_cache[i -10] * (seq_t)432;

                neg += new_cache[i - 2] * (seq_t)23;
                neg += new_cache[i - 5] * (seq_t)511;
                neg += new_cache[i - 8] * (seq_t)168;

                new_cache[i] = pos - neg;   /* ensure pos >= neg for all n */
            }
        }

        cache_size = new_len;
        cache = new_cache;
    }

    return cache[n];
}
