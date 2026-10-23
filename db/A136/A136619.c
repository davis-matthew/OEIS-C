/*
 * A136619  a(1) = 1, then repeat period 3: [1, 4, 2].
 * https://oeis.org/A136619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=1, a(2)=1, a(3)=4, a(4)=2; For n>4, a(n)=a(n-3), so the
 *            pattern [1,4,2] repeats after the first term
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)1;
    if (n == 3u) return (seq_t)4;
    if (n == 4u) return (seq_t)2;

    /* For n>=5 the sequence is periodic with period 3.
       (n-2) % 3 gives 0→1, 1→4, 2→2. */
    idx_t r = (n - 2u) % 3u;
    switch (r) {
        case 0u: return (seq_t)1;
        case 1u: return (seq_t)4;
        default: return (seq_t)2;
    }
}
