/*
 * A143967  Numbers containing only digits 3 or 7 in decimal representation.
 * https://oeis.org/A143967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the nth number whose decimal digits are only 3 or 7 (in
 *            increasing numeric order)
 * Range:     u8 n <= 6, u16 n <= 46, u32 n <= 1534, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th element of OEIS A143967.  This uses the
   observation that numbers of length L are in one‑to‑one correspondence
   with L‑bit binary numbers: replace each binary bit 0 with digit 3
   and each bit 1 with digit 7.  The sequence is simply a lexicographic
   enumeration of these binary patterns sorted by numeric value. */
seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; for n == 0 return 0 as a safe
       fallback – the caller must never ask for n == 0. */
    if (n == 0) return 0;

    idx_t cumulative = 0;   /* number of terms of lengths < L */
    idx_t power = 1;        /* 2^0 initially; in loop becomes 2^L */
    idx_t L = 0;            /* current length being considered */

    /* Find the smallest L such that the cumulative number of terms
       up to length L is at least n.  This is the length of the
       desired number. */
    while (1) {
        power <<= 1;       /* power = 2^L */
        ++L;
        if (cumulative + power >= n)
            break;
        cumulative += power;
    }

    /* Offset within the block of length L (0‑based). */
    idx_t offset = n - cumulative - 1;

    /* Convert the binary index offset into the decimal number
       consisting only of digits 3 and 7. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        idx_t bit = (offset >> (L - 1 - i)) & 1;
        seq_t digit = 3 + 4 * bit;   /* 3 if bit==0, 7 if bit==1 */
        val = val * 10 + digit;
    }

    return val;
}
