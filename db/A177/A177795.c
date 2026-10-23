/*
 * A177795  Number of length n binary words that have at least one maximal run
 *          of 1's having length two.
 * https://oeis.org/A177795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A177795: Number of binary words of length n that contain
 * at least one maximal run of 1's having length two.
 *
 * Formula (for n ≥ 5):
 *   a(n) = 2*a(n-1) + 2^(n-4) - a(n-3) + a(n-4)
 * with initial values
 *   a(1)=0, a(2)=1, a(3)=2, a(4)=5.
 *
 * The function recomputes the value from scratch on each call.
 * It only uses a few scalar variables, so the running time is O(n)
 * and the memory usage is O(1).  It works for any seq_t that supports
 * the usual arithmetic and bit‑shift operators (including arbitrary‑
 * precision big‑integer types). */

seq_t a(idx_t n)
{
    /* Handle the first four indices directly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)5;

    /* Rolling window of the last four values:
     *  a_i4 = a(i-4), a_i3 = a(i-3), a_i2 = a(i-2), a_i1 = a(i-1) */
    seq_t a_i4 = (seq_t)0;  /* a(1) */
    seq_t a_i3 = (seq_t)1;  /* a(2) */
    seq_t a_i2 = (seq_t)2;  /* a(3) */
    seq_t a_i1 = (seq_t)5;  /* a(4) */

    /* Compute successive terms up to n. */
    for (idx_t i = 5; i <= n; ++i) {
        /* 2^(i-4) via left shift */
        seq_t pow_term = ((seq_t)1) << (i - 4);

        /* Recurrence: a(i) = 2*a_i1 + pow_term - a_i3 + a_i4 */
        seq_t next = (a_i1 << 1) + pow_term - a_i3 + a_i4;

        /* Slide the window for the next iteration */
        a_i4 = a_i3;
        a_i3 = a_i2;
        a_i2 = a_i1;
        a_i1 = next;
    }

    return a_i1;
}
