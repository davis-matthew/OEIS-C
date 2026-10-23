/*
 * A037021  Greedy sequence such that no term is the average of four others.
 * https://oeis.org/A037021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 122 * A_k + R_j * where A_k = Σ bit_i(k-1) · 5^i (binary
 *            digits of (k-1) interpreted as base‑5 digits) * and R_j is taken
 *            from {0,1,2,3,5,7,13,26,27,28,29,31}
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* residues that appear in every block of 12 numbers */
    static const seq_t residues[12] = {0,1,2,3,5,7,13,26,27,28,29,31};

    /* Convert n to 0‑based index and split into block (A index) and position inside block */
    idx_t idx   = n - 1;              /* 0‑based overall index */
    idx_t block = idx / 12;           /* index of A (0,1,5,6,25,…) */
    unsigned pos = (unsigned)(idx % 12); /* index into residues */

    /* Compute A = Σ bit_i(block) · 5^i */
    seq_t A   = 0;
    seq_t p5  = 1;      /* current power of 5 = 5^i */
    idx_t tmp = block;
    while (tmp > 0) {
        if (tmp & 1)
            A += p5;
        p5 *= (seq_t)5;
        tmp >>= 1;
    }

    /* Final term: 122 * A + residue */
    return (seq_t)122 * A + residues[pos];
}
