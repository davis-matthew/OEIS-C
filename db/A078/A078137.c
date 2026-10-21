/*
 * A078137  Numbers which can be written as sum of squares>1.
 * https://oeis.org/A078137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = n + 12  for  n ≥ 12
    a(1)= 4, a(2)= 8, a(3)= 9, a(4)=12, a(5)=13,
    a(6)=16, a(7)=17, a(8)=18, a(9)=20, a(10)=21,
    a(11)=22          */

/* Return the nth term (offset 1) of A078137. */
seq_t a(idx_t n) {
    /* Hook for small n – values are hard‑coded from the given list. */
    static const seq_t small[11] = {4, 8, 9, 12, 13, 16, 17, 18, 20, 21, 22};

    if (n <= 11)           /* 1 ≤ n ≤ 11  */
        return small[n - 1];
    return (seq_t)n + 12;  /* n ≥ 12  */
}
