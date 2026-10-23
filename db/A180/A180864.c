/*
 * A180864  Trajectory of 13 under map n->A006368(n).
 * https://oeis.org/A180864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A006368:  a(0) = 13.
   For an integer x:
     if x is even → a(x) = x + x/2          (i.e., 3x/2)
     if x is odd  → a(x) = x - (x/4 + (x%4==3?1:0))
   The sequence a(n) is defined by a(n+1) = a(n) under this map. */

static inline seq_t _f(seq_t x)
{
    if ((x & 1u) == 0)          /* even */
        return x + x / 2;
    else                        /* odd */
    {
        seq_t d = x / 4 + ((x % 4u) == 3 ? 1 : 0);
        return x - d;
    }
}

seq_t a(idx_t n)
{
    seq_t val = 13;              /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        val = _f(val);
    return val;
}
