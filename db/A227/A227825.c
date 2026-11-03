/*
 * A227825  Primes whose digits add to 10 and which have a 3 in the tens
 *          place.
 * https://oeis.org/A227825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime whose decimal digits sum to 10 and whose
 *            tens digit equals 3 (offset 1)
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>        /* for size_t, NULL */
#include <stdlib.h>        /* for malloc, realloc */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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
        seq_t *new_p = mem_resize(seq_t, prime_list, new_cap);
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
