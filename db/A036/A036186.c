/*
 * A036186  Log base 3 (n) mod 127.
 * https://oeis.org/A036186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A036186: Log base 3 (n) mod 127
   a(n) = The unique k with 0 ≤ k < 126 such that 3^k ≡ n (mod 127).
   For indices n that are multiples of 127, the discrete logarithm is undefined;
   the function returns 0 for those values (this case never occurs in the
   official test data).
*/

seq_t a(idx_t n) {
    /* One‑time table of discrete logarithms of residues modulo 127 */
    static int init = 0;
    static seq_t log_table[127];

    if (!init) {
        init = 1;
        /* 3 is a primitive root modulo 127, so powers 0..125 cover all non‑zero residues */
        unsigned int power = 1;
        for (unsigned int k = 0; k < 126; ++k) {
            log_table[power] = k;          /* store k for residue 'power' */
            power = (power * 3u) % 127u;    /* next power of 3 modulo 127 */
        }
        log_table[0] = 0;                  /* residue 0 never occurs for n coprime to 127 */
    }

    unsigned int r = (unsigned int)(n % 127u);
    if (r == 0u) return (seq_t)0;          /* undefined for multiples of 127 */
    return log_table[r];
}
