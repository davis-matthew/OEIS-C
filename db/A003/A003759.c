/*
 * A003759  Number of Hamiltonian cycles in D_4 X P_n.
 * https://oeis.org/A003759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 2*a(n-3) + a(n-4), n > 4; a(1)=0,
 *            a(2)=2, a(3)=6, a(4)=24
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cache[10001];
static int cached_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 24;
    
    if (n > 10000) {
        seq_t a1 = 0, a2 = 2, a3 = 6, a4 = 24;
        for (idx_t i = 5; i <= n; i++) {
            seq_t new_a = 3 * a4 + 3 * a3 - 2 * a2 + a1;
            a1 = a2;
            a2 = a3;
            a3 = a4;
            a4 = new_a;
        }
        return a4;
    }
    
    int n_int = (int)n;
    
    if (n_int <= cached_up_to) {
        return cache[n_int];
    }
    
    if (cached_up_to == 0) {
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 24;
        cached_up_to = 4;
    }
    
    for (int i = cached_up_to + 1; i <= n_int; i++) {
        cache[i] = 3 * cache[i - 1] + 3 * cache[i - 2] - 2 * cache[i - 3] + cache[i - 4];
    }
    
    cached_up_to = n_int;
    return cache[n_int];
}
