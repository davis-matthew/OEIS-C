/*
 * A189668  Fixed point of the morphism 0->010, 1->100.
 * https://oeis.org/A189668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*k-2) = a(k); a(3*k-1) = 1 - a(k); a(3*k) = 0 (k >= 1), a(0)
 *            = 0
 * Range:     u8 n <= 122, u16 n <= 122, u32 n <= 122, u64 n <= 122,
 *            u128 n <= 122, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive helper that implements the recurrence. */
static seq_t a_rec(idx_t n)
{
    /* Base cases: a(1)=0, a(2)=1.  a(3) follows from the modulus rule. */
    if (n <= 2) {
        if (n == 1) return (seq_t)0;
        return (seq_t)1;            /* n == 2 */
    }

    /* a(3*k) = 0 */
    if (n % 3 == 0) return (seq_t)0;

    /* a(3*k-2) = a(k) */
    if (n % 3 == 1) return a_rec((n + 2) / 3);

    /* a(3*k-1) = 1 - a(k) */
    return (seq_t)1 - a_rec((n + 1) / 3);
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
