/*
 * A010411  Squares mod 50.
 * https://oeis.org/A010411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th quadratic residue modulo 50 (sorted
 *            increasingly)
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n){
    static seq_t residues[22];
    static unsigned char initialized = 0;
    if (!initialized){
        unsigned char visited[50] = {0};
        for (idx_t i = 0; i < 50; ++i){
            seq_t val = ((seq_t)i * (seq_t)i) % (seq_t)50;
            unsigned char idx = (unsigned char)val;
            visited[idx] = 1;
        }
        unsigned char pos = 0;
        for (int v = 0; v < 50; ++v){
            if (visited[v]){
                residues[pos++] = v;
            }
        }
        initialized = 1;
    }
    if (n == 0) return 0;
    if (n - 1 < 22) return residues[n-1];
    return 0;
}
