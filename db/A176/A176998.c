/*
 * A176998  a(n) = binary "k-number" for n-rowed Kayles.
 * https://oeis.org/A176998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A176998: a(n) is the binary representation of the Grundy number for an n‑pin Kayles position.
   The function computes the Grundy value G(n) via the standard mex recurrence for Kayles
   and then returns its binary digits interpreted as a decimal number 
   (e.g., 6 => 110). */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
# include <new>
#endif

/* Global cache for Grundy numbers */
static seq_t *G = NULL;        /* array of Grundy numbers */
static idx_t cache_size = 0;   /* number of entries currently stored */

/* Ensure that Grundy numbers up to n are computed and stored in G. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;                           /* already computed */

    idx_t new_size = n + 1;
    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_size];
#else
    new_arr = (seq_t*)malloc(new_size * sizeof(seq_t));
#endif

    /* Copy existing values, if any */
    if (G && cache_size > 0) {
        for (idx_t i = 0; i < cache_size; ++i)
            new_arr[i] = G[i];
    }

    /* Initialize base case */
    if (cache_size == 0)
        new_arr[0] = (seq_t)0;

    /* Compute missing entries */
    idx_t start = (cache_size == 0) ? 1 : cache_size;
    for (idx_t k = start; k <= n; ++k) {
        /* visited array sufficient for Kayles Grundy numbers up to n=10000 */
        bool visited[2048] = {false};

        /* Remove a single pin at position i (0‑based). */
        for (idx_t i = 0; i < k; ++i) {
            seq_t g = new_arr[i] ^ new_arr[k - i - 1];
            if (g < 2048)
                visited[(int)g] = true;
        }

        /* Remove two adjacent pins starting at i. */
        for (idx_t i = 0; i + 1 < k; ++i) {
            seq_t g = new_arr[i] ^ new_arr[k - i - 2];
            if (g < 2048)
                visited[(int)g] = true;
        }

        /* Mex: smallest non‑negative integer not in visited. */
        seq_t mex = 0;
        while (mex < 2048 && visited[(int)mex])
            ++mex;
        new_arr[k] = mex;
    }

    /* Replace old cache */
    if (G) {
#ifdef __cplusplus
        delete[] G;
#else
        free(G);
#endif
    }
    G = new_arr;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is not defined; return 0. */
    if (n == 0)
        return (seq_t)0;

    ensure_cache(n);
    seq_t g = G[n];

    /* Convert Grundy number to its binary representation as a decimal number. */
    unsigned bitlen = 0;
    seq_t temp = g;
    while (temp > 0) {
        ++bitlen;
        temp >>= 1;
    }

    seq_t result = (seq_t)0;
    for (int i = (int)bitlen - 1; i >= 0; --i) {
        seq_t bit = (g >> i) & (seq_t)1;
        result = result * ((seq_t)10) + bit;
    }
    return result;
}
