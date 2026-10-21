/*
 * A235583  Numbers not divisible by 2, 5 or 7.
 * https://oeis.org/A235583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer coprime to 70 (i.e., not
 *            divisible by 2, 5, or 7)
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _coprime_residues[24] = {
    (seq_t)1,(seq_t)3,(seq_t)9,(seq_t)11,
    (seq_t)13,(seq_t)17,(seq_t)19,(seq_t)23,
    (seq_t)27,(seq_t)29,(seq_t)31,(seq_t)33,
    (seq_t)37,(seq_t)39,(seq_t)41,(seq_t)43,
    (seq_t)47,(seq_t)51,(seq_t)53,(seq_t)57,
    (seq_t)59,(seq_t)61,(seq_t)67,(seq_t)69
};

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 in the specification.  We use
       a 0‑based index to compute the block and offset within each
       block of 70 numbers that contain 24 coprimes to 70. */
    if (n == 0) return (seq_t)0;   /* safety for out of bound, though not used */

    idx_t z = n - 1;                 /* zero based index */
    idx_t block = z / 24;            /* complete 70‑number blocks before the wanted number */
    idx_t offset = z % 24;           /* position inside the current block */

    /* Compute the value modulo 70 using the pre‑computed residues.
       The multiplication is carried out in seq_t to avoid overflow
       when n is large. */
    seq_t val = (seq_t)70 * (seq_t)block + _coprime_residues[offset];
    return val;
}
