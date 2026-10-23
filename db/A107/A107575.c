/*
 * A107575  a(n)=greatest sidelength of n-th triangle listed at A107572.
 * https://oeis.org/A107575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: a(n) is the greatest side length of the n-th
   integer‑sided scalene triangle (a<b<c) listed in A107572.
   The triangles are ordered first by increasing perimeter p=a+b+c,
   then lexicographically by a, then b. */

seq_t a(idx_t n)
{
    idx_t count = 0;
    /* minimal perimeter for a<b<c: 2+3+4 = 9 */
    for (idx_t p = 9; ; ++p)
    {
        /* a must be at most (p-3)/3 so that a<b<c can hold */
        for (idx_t a = 2; a <= (p - 3) / 3; ++a)
        {
            /* b ranges from a+1 up to (p-a-1)/2 to keep b<c */
            for (idx_t b = a + 1; b <= (p - a - 1) / 2; ++b)
            {
                idx_t c = p - a - b;
                /* ensure triangle inequality */
                if (a + b <= c)
                    continue;   /* not a triangle */
                /* scalene condition (a<b<c) is already satisfied */
                ++count;
                if (count == n)
                    return (seq_t)c;
            }
        }
    }
    /* unreachable */
    return 0;
}
