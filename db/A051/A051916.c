/*
 * A051916  The Greek sequence: 2^a * 3^b * 5^c where a = 0,1,2,3,..., b,c in
 *          {0,1}, excluding the terms 1,2; that is: (a,b,c) != (0,0,0),
 *          (1,0,0).
 * https://oeis.org/A051916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+4) = 2*a(n) for n>8, with initial values
 *            a(1)=3,a(2)=4,a(3)=5,a(4)=6,
 *            a(5)=8,a(6)=10,a(7)=12,a(8)=15,a(9)=16,a(10)=20,a(11)=24,a(12)=30.
 *            For n>12, a(n)=2*a(n-4) so the term can be obtained by reducing
 *            n into the range 9..12 and doubling the result appropriately
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 120, u64 n <= 248,
 *            u128 n <= 504, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values for indices 1 … 12 */
    static const seq_t init[12] = {3,4,5,6,8,10,12,15,16,20,24,30};

    if (n <= 12)
        return init[n - 1];

    /* Find the base index (9‑12) sharing the same residue modulo 4 as n. */
    idx_t base;
    switch (n % 4) {
        case 0: base = 12; break;   /* n ≡ 0 (mod 4) → a(12), a(16), … */
        case 1: base = 9;  break;   /* n ≡ 1 (mod 4) → a(9), a(13), …  */
        case 2: base = 10; break;   /* n ≡ 2 (mod 4) → a(10), a(14), … */
        default: base = 11;         /* n ≡ 3 (mod 4) → a(11), a(15), … */
    }

    /* Number of doublings needed to reach a(n). */
    idx_t doublings = (n - base) / 4;

    /* Start from the known value a(base). */
    seq_t val = init[base - 1];

    /* Multiply by 2 a number of times equal to the number of 4‑step jumps. */
    for (idx_t i = 0; i < doublings; ++i)
        val += val;   /* val = 2 * val */

    return val;
}
