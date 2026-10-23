/*
 * A117987  Number of functions f:[n]->[n] such that f[(2*x) mod n]=[2*f(x)]
 *          mod n for all x in [n], for n=1,2,3,... Here [n] denotes
 *          {0,1,2,...,n-1}.
 * https://oeis.org/A117987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-m) * ∏_{cycle C of x→2x mod n} gcd(n, 2^|C|-1)
 *            (where n=2^t·m, m odd)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   gcd, mod_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* separate odd part m and exponent (n-m) */
    idx_t n_orig = n;
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;          /* m is odd, n = 2^t * m */
    idx_t pow_exp = n_orig - m;            /* n - m  */
    seq_t N = (seq_t)n_orig;               /* value of n as seq_t */

    /* start with 2^(n-m) */
    seq_t result = pow2(pow_exp);          /* pow2 from utils */

    /* allocate visited and position arrays */
    uint8_t *visited = (uint8_t*)malloc((size_t)n_orig);
    int *pos      = (int*)malloc((size_t)n_orig * sizeof(int));
    if (!visited || !pos) {
        /* allocation failed – return 0 */
        free(visited);
        free(pos);
        return 0;
    }
    memset(visited, 0, (size_t)n_orig);
    memset(pos, -1, (size_t)n_orig * sizeof(int));

    /* temporary array to reset pos[] after each component */
    idx_t *order = (idx_t*)malloc((size_t)n_orig * sizeof(idx_t));
    if (!order) {
        free(visited);
        free(pos);
        return 0;
    }

    for (idx_t i = 0; i < n_orig; ++i) {
        if (visited[i]) continue;

        /* traverse component starting from i */
        idx_t x = i;
        idx_t step = 0;
        while (!visited[x]) {
            visited[x] = 1;
            pos[x] = (int)step;
            order[step] = x;
            ++step;
            x = (2ull * x) % n_orig;
        }

        /* if we ended at an earlier node in this component, we found a cycle */
        if (pos[x] >= 0) {
            idx_t cycle_len = step - (idx_t)pos[x];

            /* compute gcd(n, 2^cycle_len - 1) */
            seq_t powmod = mod_pow((seq_t)2, cycle_len, N);   /* 2^cycle_len mod n */
            seq_t t_val = powmod + N - 1;
            if (t_val >= N) t_val -= N;
            seq_t g = gcd(N, t_val);

            result = result * g;
        }

        /* reset pos for nodes of this component */
        for (idx_t j = 0; j < step; ++j) {
            pos[order[j]] = -1;
        }
    }

    free(visited);
    free(pos);
    free(order);
    return result;
}
