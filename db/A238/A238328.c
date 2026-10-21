/*
 * A238328  Sum of all the parts in the partitions of 4n into 4 parts.
 * https://oeis.org/A238328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=4, a(2)=40, a(3)=180, a(4)=544, a(5)=1280, a(6)=2592,
 *            a(7)=4732, a(8)=7968, a(9)=12636 For n>9,
 *            a(n)=3*a(n-1)-3*a(n-2)+3*a(n-3)-6*a(n-4)+6*a(n-5)
 *            -3*a(n-6)+3*a(n-7)-3*a(n-8)+a(n-9) (Linear 9‑term recurrence)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 221, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#ifndef MAX_N
#define MAX_N 100000
#endif

/* Cached values (zero‑based index unused). */
static seq_t _a_cache[MAX_N + 1];
static size_t _cached_up_to = 0;
static int _init_done = 0;

/* Initialise the first nine terms once. */
static void _init_cache(void)
{
    if (_init_done) return;
    _a_cache[0] = 0;               /* unused */
    _a_cache[1] = 4;
    _a_cache[2] = 40;
    _a_cache[3] = 180;
    _a_cache[4] = 544;
    _a_cache[5] = 1280;
    _a_cache[6] = 2592;
    _a_cache[7] = 4732;
    _a_cache[8] = 7968;
    _a_cache[9] = 12636;
    _cached_up_to = 9;
    _init_done = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n<1 */

    _init_cache();
    size_t N = (size_t)n;

    /* Return cached value if already computed. */
    if (N <= _cached_up_to) return _a_cache[N];

    /* If n is within the pre‑allocated cache, compute forward. */
    if (N <= MAX_N) {
        for (size_t i = _cached_up_to + 1; i <= N; ++i) {
            _a_cache[i] = ((seq_t)3)*_a_cache[i-1]
                        - ((seq_t)3)*_a_cache[i-2]
                        + ((seq_t)3)*_a_cache[i-3]
                        - ((seq_t)6)*_a_cache[i-4]
                        + ((seq_t)6)*_a_cache[i-5]
                        - ((seq_t)3)*_a_cache[i-6]
                        + ((seq_t)3)*_a_cache[i-7]
                        - ((seq_t)3)*_a_cache[i-8]
                        + _a_cache[i-9];
        }
        _cached_up_to = N;
        return _a_cache[N];
    }

    /* For n beyond the cache size, use a rolling buffer. */
    seq_t buf[10];   /* buf[1..9] hold previous nine values */
    for (size_t i = 1; i <= 9; ++i) buf[i] = _a_cache[i];
    for (size_t i = 10; i <= N; ++i) {
        seq_t val = ((seq_t)3)*buf[1]
                 - ((seq_t)3)*buf[2]
                 + ((seq_t)3)*buf[3]
                 - ((seq_t)6)*buf[4]
                 + ((seq_t)6)*buf[5]
                 - ((seq_t)3)*buf[6]
                 + ((seq_t)3)*buf[7]
                 - ((seq_t)3)*buf[8]
                 + buf[9];
        /* shift buffer */
        for (size_t k = 1; k < 9; ++k) buf[k] = buf[k+1];
        buf[9] = val;
    }
    return buf[9];
}
