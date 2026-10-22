/*
 * A007816  Number of abstract simplicial 2-complexes on {1,2,3,...,n+3} which
 *          triangulate the 2-sphere: C(n+3,2)*(4n+1)!/(3n+3)!.
 * https://oeis.org/A007816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = C(n+3,2) * (4n+1)! / (3n+3)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    if (n == 1) {
        return (seq_t)1;
    }
    
    if (n == 2) {
        return (seq_t)10;
    }
    
    // For n >= 3:
    // a(n) = C(n+3,2) * (4n+1)! / (3n+3)!
    //      = (n+3) * (n+2) / 2 * (4n+1) * (4n) * ... * (3n+4)
    
    seq_t result = 1;
    
    seq_t n_plus_3 = n_seq + 3;
    seq_t n_plus_2 = n_seq + 2;
    
    // Compute C(n+3, 2) = (n+3) * (n+2) / 2
    if (n_plus_3 % 2 == 0) {
        result = (n_plus_3 / 2) * n_plus_2;
    } else {
        result = n_plus_3 * (n_plus_2 / 2);
    }
    
    // Multiply by (4n+1) * (4n) * ... * (3n+4)
    for (seq_t i = 3 * n_seq + 4; i <= 4 * n_seq + 1; i++) {
        result *= i;
    }
    
    return result;
}
