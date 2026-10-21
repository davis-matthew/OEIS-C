/*
 * A076294  Consider all Pythagorean triples (X,X+7,Z); sequence gives Z
 *          values.
 * https://oeis.org/A076294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n >= 6 * a(0)=5, a(1)=7, a(2)=13,
 *            a(3)=17, a(4)=35, a(5)=73
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 73, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n < 6) {
        switch ((int)n) {
            case 0: return (seq_t)5;
            case 1: return (seq_t)7;
            case 2: return (seq_t)13;
            case 3: return (seq_t)17;
            case 4: return (seq_t)35;
            default: /* n == 5 */ return (seq_t)73;
        }
    }

    /* Sliding window for the linear recurrence */
    seq_t t[6] = { (seq_t)5, (seq_t)7, (seq_t)13,
                   (seq_t)17, (seq_t)35, (seq_t)73 };

    for (idx_t i = 6; i <= n; ++i) {
        idx_t r    = i % 6;          /* position to store a(i)   */
        idx_t idx1 = (i - 3) % 6;    /* a(i-3)                     */
        idx_t idx2 = (i - 6) % 6;    /* a(i-6)                     */
        t[r] = (seq_t)6 * t[idx1] - t[idx2];
    }
    return t[n % 6];
}
