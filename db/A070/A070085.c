/*
 * A070085  a(n) = A070080(n)^2 + A070081(n)^2 - A070082(n)^2.
 * https://oeis.org/A070085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A070080(n)^2 + A070081(n)^2 - A070082(n)^2 where
 *            (A070080(n),A070081(n),A070082(n)) is the n‑th integer triangle
 *            enumerated by increasing perimeter and lexicographic order of
 *            its sides
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t p = 3; ; p++)
    {
        for (int a = 1; a <= p/3; a++)
        {
            int b_min = a;
            int b_max = (p - a) / 2;
            for (int b = b_min; b <= b_max; b++)
            {
                int c = p - a - b;
                if (a + b <= c) continue;          /* not a triangle */
                count++;
                if (count == n)
                {
                    seq_t Ai = (seq_t)a;
                    seq_t Bi = (seq_t)b;
                    seq_t Ci = (seq_t)c;
                    return Ai*Ai + Bi*Bi - Ci*Ci;
                }
            }
        }
    }
}
