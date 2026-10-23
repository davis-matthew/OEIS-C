/*
 * A042518  Numerators of continued fraction convergents to sqrt(788).
 * https://oeis.org/A042518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 786*a(n-2) - a(n-4) for n >= 4, with a(0) = 28, a(1) =
 *            393, a(2) = 22036, a(3) = 308897
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)28;
    if (n == 1) return (seq_t)393;
    if (n == 2) return (seq_t)22036;
    if (n == 3) return (seq_t)308897;

    /* Even indices: use recurrence on e(k) = a(2k) */
    if ((n & 1ULL) == 0) {
        idx_t k = n >> 1;           /* k = n / 2 */
        seq_t prevprev = (seq_t)28; /* e(0) */
        seq_t prev     = (seq_t)22036;  /* e(1) */
        for (idx_t i = 2; i <= k; ++i) {
            seq_t curr = (seq_t)786 * prev - prevprev;
            prevprev = prev;
            prev = curr;
        }
        return prev;
    }

    /* Odd indices: use recurrence on o(k) = a(2k+1) */
    {
        idx_t k = (n - 1) >> 1;      /* k = (n-1)/2 */
        seq_t prevprev = (seq_t)393; /* o(0) */
        seq_t prev     = (seq_t)308897;/* o(1) */
        for (idx_t i = 2; i <= k; ++i) {
            seq_t curr = (seq_t)786 * prev - prevprev;
            prevprev = prev;
            prev = curr;
        }
        return prev;
    }
}
