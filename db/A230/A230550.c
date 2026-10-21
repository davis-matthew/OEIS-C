/*
 * A230550  Voids left after packing twin hearts patterns into n X n coins.
 * https://oeis.org/A230550
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230550: Voids left after packing twin hearts patterns into n×n coins.
   Computed by the recurrence
   a(n) = a(n−2) + a(n−3) − a(n−5) + a(n−6) − a(n−8) − a(n−9) + a(n−11)
   for n ≥ 13, with a(2) … a(12) given below. */

static const seq_t init[11] = { 1, 2, 5, 10, 13, 22, 33, 40, 51, 68, 73 };

seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* value undefined for n < 2 */
    if (n <= 12) return init[(idx_t)(n - 2)];

    seq_t last[11];
    for (int i = 0; i < 11; ++i) last[i] = init[i];

    for (idx_t curr = 13; curr <= n; ++curr)
    {
        /* a(curr) = a(curr-2)+a(curr-3)-a(curr-5)+a(curr-6)-a(curr-8)-a(curr-9)+a(curr-11) */
        seq_t pos = last[9] + last[8] + last[5] + last[0];
        seq_t neg = last[6] + last[3] + last[2];
        seq_t val = pos - neg;

        /* shift buffer left, drop a(curr-11) */
        for (int i = 0; i < 10; ++i) last[i] = last[i + 1];
        last[10] = val;
    }
    return last[10];
}
