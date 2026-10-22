/*
 * A005612  Number of Boolean functions of n variables that are variously
 *          called "unate cascades" or "1-decision list functions" or
 *          "read-once threshold functions".
 * https://oeis.org/A005612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; for n > 1, a(n) = 2^(n+1) * (P_n - n*P_{n-1}) where
 *            P_n is the n-th Fubini number (A000670) computed as sum of
 *            Stirling numbers times factorials
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   compute_fubini_pair, pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/pw2.h"
#include "../utils/compute_fubini_pair.h"

seq_t a(idx_t n) {
    if (n == 1) return 2;
    
    seq_t P_n, P_n_minus_1;
    compute_fubini_pair(n, &P_n, &P_n_minus_1);
    
    seq_t power_of_2 = pw2(n + 1);
    seq_t result = power_of_2 * (P_n - (seq_t)n * P_n_minus_1);
    
    return result;
}
