/*
 * A236138  a(n) = |{0 < k < n: p = phi(k) + phi(n-k)/3 - 1, prime(p-1) -
 *          (p-1) and prime(p-1) - 2*prime((p-1)/2) are all prime}|, where
 *          phi(.) is Euler's totient function.
 * https://oeis.org/A236138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{0 < k < n : p = φ(k) + φ(n-k)/3 - 1, with φ the Euler
 *            totient; prime(p-1) - (p-1) and prime(p-1) - 2·prime((p-1)/2)
 *            both prime}| The algorithm fills a φ array up to the requested n
 *            (sieve of Euler) and then scans all k. It rejects any k for
 *            which φ(n-k) is not divisible by 3, because in...
 * Range:     u8 n <= 95, u16 n <= 5726, u32 n <= 7868, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"

/* storage for Euler's totient values */
static seq_t *phi_arr = NULL;
static idx_t  phi_max = 0;

/* build φ[0 … n] */
static void build_phi(idx_t n)
{
    if (phi_arr == NULL) {
        phi_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else if (n > phi_max) {
        phi_arr = (seq_t *)realloc(phi_arr, (n + 1) * sizeof(seq_t));
    } else {
        return;
    }

    for (idx_t i = 0; i <= n; ++i) phi_arr[i] = (seq_t)i;

    for (idx_t p = 2; p <= n; ++p) {
        if (phi_arr[p] == (seq_t)p) {          /* p is prime */
            for (idx_t j = p; j <= n; j += p)
                phi_arr[j] -= phi_arr[j] / (seq_t)p;
        }
    }
    phi_max = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* out of range safety */

    if (n > phi_max) build_phi(n);          /* ensure φ up to n */

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t pk   = phi_arr[k];
        seq_t pnk  = phi_arr[n - k];
        /* φ(n-k) must be divisible by 3 */
        if (pnk % (seq_t)3 != 0) continue;
        seq_t p = pk + pnk / (seq_t)3 - (seq_t)1;
        if (p < (seq_t)2) continue;         /* p must be prime */

        /* p itself must be prime */
        if (!is_prime_big(p)) continue;

        /* p-1 must be a valid prime index */
        if (p <= (seq_t)1) continue;
        idx_t p_minus1 = (idx_t)(p - (seq_t)1);

        seq_t prime_p_minus1 = get_prime(p_minus1);
        seq_t val1 = prime_p_minus1 - (seq_t)p_minus1;
        if (val1 < (seq_t)2 || !is_prime_big(val1)) continue;

        /* (p-1)/2 must be integer and >= 1 */
        if ((p_minus1 & (seq_t)1) != 0) continue;  /* odd */
        idx_t half = (idx_t)(p_minus1 / (seq_t)2);
        if (half < (idx_t)1) continue;

        seq_t prime_half = get_prime(half);
        seq_t val2 = prime_p_minus1 - (seq_t)2 * prime_half;
        if (val2 < (seq_t)2 || !is_prime_big(val2)) continue;

        ++count;
    }
    return count;
}
