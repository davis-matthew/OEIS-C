/*
 * A206116  Number of (n+1) X 7 0..3 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two counterclockwise and two clockwise edge
 *          increases.
 * https://oeis.org/A206116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2445*a(n-3) for n > 9 * Initial values: * a(1)=973388,
 *            a(2)=3048096, a(3)=10792800, * a(4)=157206240, a(5)=1990281312,
 *            a(6)=24408220320, * a(7)=373319837280, a(8)=4839970998240,
 *            a(9)=59527064527200
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 13, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)973388ULL,
        (seq_t)3048096ULL,
        (seq_t)10792800ULL,
        (seq_t)157206240ULL,
        (seq_t)1990281312ULL,
        (seq_t)24408220320ULL,
        (seq_t)373319837280ULL,
        (seq_t)4839970998240ULL,
        (seq_t)59527064527200ULL
    };

    if (n == 0) return (seq_t)0;   /* offset not defined */
    if (n <= 9) return init[n - 1];

    /* find the nearest index ≤ 9 with the same remainder mod 3 */
    idx_t mod = n % 3;
    idx_t baseIdx;
    if (mod == 0)          /* n ≡ 0 (mod 3) → use a(9) */
        baseIdx = 9;
    else if (mod == 1)     /* n ≡ 1 (mod 3) → use a(7) */
        baseIdx = 7;
    else                   /* n ≡ 2 (mod 3) → use a(8) */
        baseIdx = 8;

    seq_t result = init[baseIdx - 1];
    idx_t steps = (n - baseIdx) / 3;
    const seq_t mul = (seq_t)2445;

    for (idx_t i = 0; i < steps; ++i)
        result *= mul;

    return result;
}
