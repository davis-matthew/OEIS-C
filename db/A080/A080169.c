/*
 * A080169  Numbers that are cubes of primes of the form 4k+1 (A002144).
 * https://oeis.org/A080169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p_n)^3 where p_n is the n‑th prime congruent to 1 (mod
 *            4)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 123, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Return the n‑th term of A080169 (1‑based). */
seq_t a(idx_t n)
{
    /* ---------- Static state retained across calls ---------- */
    static idx_t *prime_list      = NULL;   /* all primes found so far   */
    static idx_t  prime_cnt       = 0;      /* number of stored primes   */
    static size_t prime_cap       = 0;      /* capacity of prime_list    */
    static seq_t *cube_list       = NULL;   /* cubes of primes ≡1 (mod 4)*/
    static idx_t  cube_cnt        = 0;      /* number of stored cubes    */
    static size_t cube_cap        = 0;      /* capacity of cube_list     */
    static idx_t  candidate       = 2;      /* next integer to test     */

    /* Helper: grow an array of idx_t when full */
    if (prime_list == NULL) {
        prime_cap   = 16;
        prime_list  = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    }
    if (cube_list == NULL) {
        cube_cap    = 16;
        cube_list   = (seq_t *)malloc(cube_cap * sizeof(seq_t));
    }

    /* ---------- Generate primes until we have at least n qualifying ones ---------- */
    while (cube_cnt < n) {
        idx_t p = candidate;

        /* Simple trial‑division using the primes we already found */
        int is_prime = 1;
        for (idx_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= p; ++i) {
            if (p % prime_list[i] == 0) {
                is_prime = 0;
                break;
            }
        }

        /* Update candidate for next iteration */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;   /* skip even numbers */

        if (!is_prime)
            continue;          /* look at the next candidate */

        /* Store the new prime */
        if (prime_cnt == prime_cap) {
            prime_cap <<= 1;
            prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
        }
        prime_list[prime_cnt++] = p;

        /* If p ≡ 1 (mod 4), record its cube */
        if ((p & 3U) == 1) {
            if (cube_cnt == cube_cap) {
                cube_cap <<= 1;
                cube_list = (seq_t *)realloc(cube_list, cube_cap * sizeof(seq_t));
            }
            seq_t pp  = (seq_t)p * (seq_t)p;
            cube_list[cube_cnt++] = pp * (seq_t)p;
        }
    }

    /* ---------- Return the required term ---------- */
    return cube_list[n - 1];
}
