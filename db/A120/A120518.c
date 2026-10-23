/*
 * A120518  a(n) = min{j : A120507(j) = n}.
 * https://oeis.org/A120518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-1 + a(floor((n-1)/4)+1),  a(1) = 1   (offset 1) */
seq_t a(idx_t n)
{
    /* Guard against an unexpected call with n == 0.  It is not part
       of the declared offset but this makes the function safe in
       all contexts.  */
    if (n == 0) {
        return (seq_t)0;
    }

    if (n == 1) {
        return (seq_t)1;
    }

    /* Compute the index of the recursive call:
       j = floor((n-1)/4) + 1.  We stay in idx_t until the final
       conversion to seq_t.  */
    idx_t j = (n - 1) / 4 + 1;

    /* Return the recurrence value.  The subtraction (n-1) and the
       recursive result are both converted to seq_t before the
       addition.  This works for plain unsigned integer types
       (uint8_t … uint64_t) and for the arbitrary‑precision types
       used in the C++ builds.  */
    return (seq_t)(n - 1) + a(j);
}
