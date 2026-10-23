/*
 * A030673  Smallest cube that begins with n-th prime.
 * https://oeis.org/A030673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest cube that begins with the n-th prime
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 82, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root, get_prime, pow10, seq_cmp, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow10.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    uint32_t p = get_prime(n);
    seq_t prime = (seq_t)p;
    seq_t prime_plus_one = prime + (seq_t)1;

    for (idx_t k = 0;; ++k)
    {
        seq_t powk  = pow10(k);
        seq_t lowbd = seq_mul(prime, powk);
        seq_t upbd  = seq_mul(prime_plus_one, powk);

        seq_t root  = floor_cube_root(lowbd);
        seq_t cube  = seq_mul(seq_mul(root, root), root);

        if (seq_cmp(&cube, &lowbd) < 0)
        {
            root = root + (seq_t)1;
            cube = seq_mul(seq_mul(root, root), root);
        }

        if (seq_cmp(&cube, &upbd) < 0)
            return cube;
        /* otherwise loop to next k */
    }
}
