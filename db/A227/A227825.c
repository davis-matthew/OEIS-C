/*
 * A227825  Primes whose digits add to 10 and which have a 3 in the tens
 *          place.
 * https://oeis.org/A227825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose decimal digits sum to 10 and whose
 *            tens digit equals 3 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 20, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t, NULL */
#include <stdlib.h>        /* for malloc, realloc */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */

static seq_t *prime_list = NULL;   /* array of found primes */
static size_t prime_cnt  = 0;       /* number of primes stored */
static size_t prime_cap  = 0;       /* allocated capacity */
static int    next_len   = 2;       /* next digit length to generate */
static uint8_t digits[64];          /* reversed digits buffer */

/* Append a prime to the dynamic array                       */
static void append_prime(seq_t val)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        seq_t *new_p = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!new_p) exit(EXIT_FAILURE);   /* allocation failed */
        prime_list = new_p;
        prime_cap  = new_cap;
    }
    prime_list[prime_cnt++] = val;
}

/* Recursive generation of all numbers with digit length L
   that satisfy:
   - tens digit == 3
   - sum of all digits == 10
   - most significant digit > 0
   For each complete number, test primality and store if prime. */
static void gen_rec(int pos, int sum_rem, int L)
{
    if (pos == 0) {                               /* set units digit */
        if (sum_rem >= 0 && sum_rem <= 9) {
            digits[0] = (uint8_t)sum_rem;
            seq_t val = 0;
            for (int i = L - 1; i >= 0; --i)
                val = val * (seq_t)10 + (seq_t)digits[i];
            if (is_prime_big(val))
                append_prime(val);
        }
        return;
    }
    if (pos == 1) {                               /* tens digit fixed to 3 */
        digits[1] = 3;
        gen_rec(pos - 1, sum_rem, L);
        return;
    }
    int min_d = (pos == L - 1) ? 1 : 0;          /* most significant >0 */
    for (int d = min_d; d <= 9 && d <= sum_rem; ++d) {
        digits[pos] = (uint8_t)d;
        gen_rec(pos - 1, sum_rem - d, L);
    }
}

/* Ensure that the list contains at least n primes                */
static void ensure_primes(idx_t n)
{
    while (prime_cnt < (size_t)n) {
        int L = next_len;                       /* current digit length   */
        gen_rec(L - 1, 7, L);                   /* remaining sum = 10-3   */
        next_len = L + 1;
    }
}

/* Required sequence function                                  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1            */
    ensure_primes(n);
    return prime_list[n - 1];
}
