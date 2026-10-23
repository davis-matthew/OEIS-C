/*
 * A023689  Numbers with exactly 7 ones in binary expansion.
 * https://oeis.org/A023689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Formula: a(1) = 127, a(n+1) = next_combination(a(n))
 *   (next_combination generates the next integer with the same popcount)
 */

/* next_combination : return the next larger integer with the same number of 1‑bits */
static inline seq_t next_combination(seq_t x)
{
    seq_t smallest = x & -(seq_t)x;            /* least significant set bit */
    seq_t ripple   = x + smallest;             /* add ripple */
    seq_t ones     = x ^ ripple;               /* ones that flipped */
    ones = (ones >> 2) / smallest;             /* recombine */
    return ripple | ones;
}

/* a : return the nth integer whose binary representation contains exactly seven 1‑bits */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 by the caller */
    seq_t value = (seq_t)127;     /* 0b1111111, a(1) */
    if (n == 1)
        return value;

    for (idx_t i = 2; i <= n; ++i)
        value = next_combination(value);

    return value;
}
