/*
 * A089356  Smallest prime beginning with digit reversal of n and not included
 *          earlier.
 * https://oeis.org/A089356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest prime not used before that begins with the
 *            decimal digits of reverse(n)
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *used = NULL;          /* array of primes already returned   */
static size_t used_size = 0;        /* number of elements in used        */
static size_t used_cap  = 0;        /* capacity of allocated array        */

/* append a prime to the used list, expanding storage as needed */
static void used_append(seq_t p) {
    if (used_size == used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 16;
        seq_t *tmp = (seq_t*)realloc(used, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* abort on allocation failure      */
        used = tmp;
        used_cap = new_cap;
    }
    used[used_size++] = p;
}

/* linear search to see if a prime has already been used */
static bool used_contains(seq_t p) {
    for (size_t i = 0; i < used_size; ++i)
        if (used[i] == p) return true;
    return false;
}

/* reverse decimal digits of an unsigned integer (seq_t) */
static seq_t rev_digits(seq_t n) {
    seq_t rev = 0;
    const seq_t TEN = (seq_t)10;
    while (n > 0) {
        seq_t digit = n % TEN;
        rev = rev * TEN + digit;
        n   = n / TEN;
    }
    return rev;
}

/* 64‑bit modular multiplication using 128‑bit intermediate */
static unsigned long long mul_mod(unsigned long long a,
                                 unsigned long long b,
                                 unsigned long long mod) {
    return (unsigned long long)((__int128)a * b % mod);
}

/* 64‑bit modular exponentiation */
static unsigned long long mod_pow(unsigned long long base,
                                  unsigned long long exp,
                                  unsigned long long mod) {
    unsigned long long result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller–Rabin primality test for 64‑bit numbers */
static bool is_prime(seq_t n) {
    unsigned long long m = (unsigned long long)n;
    if (m < 2)                 return false;
    if (m % 2 == 0)            return m == 2;
    if (m % 3 == 0)            return m == 3;

    unsigned long long d = m - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    const unsigned long long witnesses[7] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};

    for (int i = 0; i < 7; ++i) {
        unsigned long long a = witnesses[i];
        if (a % m == 0) continue;
        unsigned long long x = mod_pow(a, d, m);
        if (x == 1 || x == m - 1) continue;
        bool better = false;
        for (unsigned long long r = 1; r < s; ++r) {
            x = mul_mod(x, x, m);
            if (x == m - 1) {
                better = true;
                break;
            }
        }
        if (!better) return false;
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n) {
    seq_t rev = rev_digits(n);
    const seq_t TEN = (seq_t)10;
    seq_t pow10 = 1;

    while (true) {
        seq_t start = rev * pow10;
        seq_t end   = (rev + 1) * pow10 - 1;

        for (seq_t x = start; x <= end; ++x) {
            if (is_prime(x) && !used_contains(x)) {
                used_append(x);
                return x;
            }
        }
        /* advance to next bigger power of 10 */
        pow10 = pow10 * TEN;
    }
}
