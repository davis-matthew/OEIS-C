/*
 * A070332  Smallest nontrivial power of n ending in n and greater than n; or
 *          0 if no such number exists.
 * https://oeis.org/A070332
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k ≥ 2 such that n^k ≡ n (mod 10^digits(n)).
 *            Return n^k (as seq_t) or 0 if no such k exists
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 11, u128 n <= 71,
 *            big any n
 * Helpers:   big_pow, digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include "../utils/pow10.h"
#include "../utils/big_pow.h"
#include "../utils/digits_of.h"

static bool visited[1000001] = {0};

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* safety for unexpected calls */

    /* number of decimal digits of n */
    int d = digits_of(n);
    seq_t mod = pow10(d);                     /* 10^d */
    seq_t n_seq = (seq_t)n;
    seq_t target = n_seq;

    /* modulus fits in int (for n up to ~10^6). */
    size_t mod_sz = (size_t)mod;
    if (mod_sz > sizeof(visited))
        mod_sz = sizeof(visited) / sizeof(visited[0]);

    memset(visited, 0, mod_sz);               /* reset visited residues */

    /* starting with k = 2 */
    seq_t r = (n_seq * n_seq) % mod;
    idx_t k = 2;

    for (;; ++k) {
        if (r == target) {
            /* found smallest k, compute the full power */
            return big_pow(n_seq, (seq_t)k);
        }
        size_t idx = (size_t)r;
        if (idx >= mod_sz)   /* should not happen for valid n */
            return (seq_t)0;
        if (visited[idx])
            return (seq_t)0;   /* cycle without hitting target */
        visited[idx] = true;
        r = (r * n_seq) % mod;  /* advance to next power */
    }
}
