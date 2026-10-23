/*
 * A026313  Numbers k such that |sin(k)*sin(k+2)| < (sin(k+1))^2.
 * https://oeis.org/A026313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k satisfying |sin(k)*sin(k+2)| <
 *            (sin(k+1))^2. Equivalent condition: cos(2k+2) < (1+cos(2))/2
 * Range:     u8 n <= 152, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define COS_DELTA  -0.4161468365471424L   /* cos(2)   */
#define SIN_DELTA   0.9092974268256817L   /* sin(2)   */
#define THRESH      0.2919265817264288L   /* (1+cos(2))/2 */

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;          /* stored sequence values */
    static size_t seq_sz = 0;            /* number of values stored */
    static size_t seq_cap = 0;           /* allocated capacity   */
    static unsigned long long next_k = 0;/* next integer k to test */
    static long double cur_cos = COS_DELTA; /* cos(2k+2)  for current k */
    static long double cur_sin = SIN_DELTA; /* sin(2k+2)  for current k */
    static int initialized = 0;

    if (!initialized) {
        seq_cap  = 128;
        seq      = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_sz   = 0;
        next_k   = 0;
        cur_cos  = COS_DELTA;
        cur_sin  = SIN_DELTA;
        initialized = 1;
    }

    /* Generate terms until we have at least n values */
    while (seq_sz < n) {
        if (cur_cos < THRESH) {
            if (seq_sz >= seq_cap) {
                seq_cap *= 2;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_sz++] = (seq_t)next_k;
        }

        /* Advance to next k using trig addition formulas */
        long double next_cos = cur_cos * COS_DELTA - cur_sin * SIN_DELTA;
        long double next_sin = cur_sin * COS_DELTA + cur_cos * SIN_DELTA;
        cur_cos = next_cos;
        cur_sin = next_sin;
        ++next_k;
    }

    return seq[n-1];
}
