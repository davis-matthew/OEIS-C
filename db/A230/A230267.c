/*
 * A230267  Coins left after packing 5 curves coins patterns into fountain of
 *          coins base n.
 * https://oeis.org/A230267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) for n ≥ 7
   Initial values (offset 1): a(1)=1, a(2)=3, a(3)=2, a(4)=6, a(5)=7, a(6)=9 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly. */
    if (n <= 6) {
        static const seq_t base[] = {0, 1, 3, 2, 6, 7, 9};
        return base[n];
    }

    /* Keep only the last six values of the sequence.
       The array index k holds a(k) for k ≡ i (mod 7). */
    seq_t last[7] = {0, 1, 3, 2, 6, 7, 9};

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = last[(i - 1) % 7] + last[(i - 2) % 7]
                    - last[(i - 4) % 7] - last[(i - 5) % 7]
                    + last[(i - 6) % 7];
        last[i % 7] = val;
    }

    return last[n % 7];
}
