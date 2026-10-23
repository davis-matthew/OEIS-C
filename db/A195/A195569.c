/*
 * A195569  Numerators b(n) of Pythagorean approximations b(n)/a(n) to 7/4.
 * https://oeis.org/A195569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 257*a(n-3) + 257*a(n-6) – a(n-9) (n ≥ 10) * with a(0) = 1
 *            and the initial values * a(1)=4, a(2)=15, a(3)=209, a(4)=1116,
 *            a(5)=3807, * a(6)=53985, a(7)=287844, a(8)=982255, a(9)=13927857
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#   include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> data;
#else
    static seq_t *data = NULL;
    static size_t cap   = 0;
#endif
    static size_t last = 0;               /* highest index already computed */

    /* Ensure we have enough space for index n and for the first ten values. */
    size_t needed = (size_t)n + 1;
    if (needed < 10) needed = 10;
#ifdef __cplusplus
    if (data.size() < needed) data.resize(needed);
#else
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(data, needed * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure – return 0 */
        data = tmp;
        cap  = needed;
    }
#endif

    /* First time: initialise the first ten terms. */
    if (last == 0) {
        /* we already have at least size 10 because of the block above */
#ifdef __cplusplus
        data[0] = (seq_t)1;
        data[1] = (seq_t)4;
        data[2] = (seq_t)15;
        data[3] = (seq_t)209;
        data[4] = (seq_t)1116;
        data[5] = (seq_t)3807;
        data[6] = (seq_t)53985;
        data[7] = (seq_t)287844;
        data[8] = (seq_t)982255;
        data[9] = (seq_t)13927857;
#else
        data[0] = (seq_t)1;
        data[1] = (seq_t)4;
        data[2] = (seq_t)15;
        data[3] = (seq_t)209;
        data[4] = (seq_t)1116;
        data[5] = (seq_t)3807;
        data[6] = (seq_t)53985;
        data[7] = (seq_t)287844;
        data[8] = (seq_t)982255;
        data[9] = (seq_t)13927857;
#endif
        last  = 9;
    }

    /* If the requested index is already known, just return it. */
    if ((size_t)n <= last) {
        return data[n];
    }

    /* Compute new terms up to n using the recurrence. */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)257 * data[i-3] + (seq_t)257 * data[i-6] - data[i-9];
        data[i]   = val;
    }
    last = (size_t)n;
    return data[n];
}
