/*
 * A134071  Primes in A134072.
 * https://oeis.org/A134071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn, base
 * Formula:   a(n) = nth prime term of A134072 where A134072(i) = concat(L_i,
 *            L_{i-1}, … , L_1) (L_k is the k‑th Lucas number: L_0 = 2, L_1 =
 *            1, L_k = L_{k-1}+L_{k-2})
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"          /* seq_t pow10(idx_t exp); */
#include "../utils/digit_len.h"      /* int digit_len(seq_t x); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* cache of found prime terms */
static seq_t *prime_terms = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* state of the construction of A134072 */
static idx_t  luc_index  = 1; /* current i, value for current_val */
static seq_t  luc_prev2  = 2; /* L_{i-1} when i >= 1; initially L0 */
static seq_t  luc_prev1  = 1; /* L_i ; initially L1 */
static seq_t  current_val = 1; /* A134072(i) for current i */

/* store a new prime term, grow array if needed */
static void cache_prime(seq_t val)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        seq_t *new_arr = (seq_t *)realloc(prime_terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
        prime_terms = new_arr;
        prime_cap   = new_cap;
    }
    prime_terms[prime_count++] = val;
}

/* generate A134072 values until we have at least `need` prime terms */
static void ensure_primes(idx_t need)
{
    if (need <= prime_count) return;

    while (prime_count < need) {
        /* compute next Lucas number L_{luc_index+1} */
        seq_t next_luc = luc_prev1 + luc_prev2;
        luc_prev2 = luc_prev1;
        luc_prev1 = next_luc;

        /* prepend next_luc to the decimal representation of current_val */
        idx_t  digit_exp = (idx_t)digit_len(current_val);
        seq_t  power     = pow10(digit_exp);
        seq_t  new_val   = next_luc * power + current_val;
        current_val = new_val;
        ++luc_index;

        if (is_prime_big(new_val))
            cache_prime(new_val);
    }
}

/* requested nth prime term of A134072 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1 */
    ensure_primes(n);
    return prime_terms[n - 1];
}
