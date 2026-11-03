/*
 * A050668  Primes p such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 6.
 * https://oeis.org/A050668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A050668 by generating primes in ascending order
   and counting the number of distinct single‑digit variations that are
   also prime. A prime enters the sequence if exactly six such variations
   exist. The function maintains a static cache of the sequence values
   so that repeated calls require no recomputation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/num_digits.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;          /* cache of sequence values      */
    static size_t seq_len = 0;         /* number of values cached       */
    static size_t seq_cap = 0;         /* allocated capacity            */
    static idx_t last_candidate = 2;   /* next integer to test          */

    /* ensure the cache is allocated */
    if (seq_len == 0) {
        seq_cap = 16;
        seq = mem_new(seq_t, seq_cap);
        if (!seq) exit(EXIT_FAILURE);
    }

    /* Generate sequence values until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* find next prime candidate */
        seq_t cand = (seq_t)last_candidate;
        last_candidate++;               /* next number to check            */

        /* skip non‑primes */
        if (!is_prime_big(cand))
            continue;

        /* count the number of prime neighbours obtained by changing
           exactly one decimal digit, keeping the same digit length   */
        int len = num_digits(cand);
        int count = 0;

        for (int pos = 0; pos < len; ++pos) {
            int exp = len - 1 - pos;                /* 10^exp */
            seq_t pow = pow10((idx_t)exp);
            seq_t q = cand / pow;
            int orig_digit = (int)(q % 10);      /* digit at position pos */

            for (int d = 0; d <= 9; ++d) {
                if (d == orig_digit) continue;
                /* disallow leading zero */
                if (pos == 0 && d == 0) continue;

                /* compute new number by increasing or decreasing that digit */
                int diff = d - orig_digit;
                seq_t new_q;
                if (diff > 0)
                    new_q = cand + (seq_t)diff * pow;
                else
                    new_q = cand - (seq_t)(-diff) * pow;

                if (is_prime_big(new_q))
                    ++count;
            }
        }

        /* if exactly six variations are prime, store the prime */
        if (count == 6) {
            if (seq_len + 1 > seq_cap) {
                seq_cap = seq_cap * 2;
                seq = mem_resize(seq_t, seq, seq_cap);
                if (!seq) exit(EXIT_FAILURE);
            }
            seq[seq_len++] = cand;
        }
    }

    return seq[n - 1];
}
