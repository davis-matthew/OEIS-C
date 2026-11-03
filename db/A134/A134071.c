/*
 * A134071  Primes in A134072.
 * https://oeis.org/A134071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn, base
 * Formula:   a(n) = nth prime term of A134072 where A134072(i) = concat(L_i,
 *            L_{i-1}, … , L_1) (L_k is the k‑th Lucas number: L_0 = 2, L_1 =
 *            1, L_k = L_{k-1}+L_{k-2})
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        seq_t *new_arr = mem_resize(seq_t, prime_terms, new_cap);
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
        idx_t  digit_exp = (idx_t)num_digits(current_val);
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
