/*
 * A037079  In ternary expansion of n, reading from left to right, digits
 *          occur in order ...,0,1,2,0,1,2,...
 * https://oeis.org/A037079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer whose ternary digits form a contiguous
   block of the repeating sequence 0,1,2,0,1,2,...  (digits increment
   by +1 modulo 3).  */
seq_t a(idx_t n)
{
    /* Handle the first three trivial numbers. */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    /* We now generate numbers of increasing length.
       For length L ≥ 2 the two admissible numbers are:
          start 1 : 1,2,0,1,…  (value v1)
          start 2 : 2,0,1,2,…  (value v2)
       Each subsequent length is obtained by multiplying by 3
       and adding the next digit (which alternates 0,1,2).  */

    idx_t count = 3u;     /* index of the next term to generate          */
    idx_t length = 2u;    /* current number of digits in v1 and v2       */
    seq_t v1 = (seq_t)5;  /* 12₂ : digits 1,2  → decimal 5             */
    seq_t v2 = (seq_t)6;  /* 20₂ : digits 2,0  → decimal 6             */

    while (1) {
        /* Output the first number of this length. */
        if (count == n) return v1;
        ++count;

        /* Output the second number of this length. */
        if (count == n) return v2;
        ++count;

        /* Prepare values for the next length.  Next digit for start 1
           is (1 + length) % 3; for start 2 it is (2 + length) % 3. */
        v1 = v1 * (seq_t)3 + (seq_t)((1u + length) % 3u);
        v2 = v2 * (seq_t)3 + (seq_t)((2u + length) % 3u);
        ++length;
    }
}
