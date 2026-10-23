/*
 * A168168  Palindromic primes with d digits which have more than 3*d/2
 *          distinct primes as substrings, for any d > 0.
 * https://oeis.org/A168168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below computes OEIS A168168 – palindromic primes with d
   digits that contain more than 3*d/2 distinct prime substrings.
   It generates palindromes in increasing order, tests each
   for primality and the substring condition, and stores the
   qualifying primes in a static table. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static seq_t *seq_vals = NULL;   /* stored sequence values          */
static size_t   seq_cnt = 0;     /* number of values stored         */
static size_t   seq_cap = 0;     /* allocated capacity              */

/* enlarge the sequence storage as needed */
static void ensure_terms_cap(size_t need)
{
    if (seq_cap >= need) return;
    size_t cap = seq_cap ? seq_cap * 2 : 16;
    while (cap < need) cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_vals, cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    seq_vals = tmp;
    seq_cap = cap;
}

/* store one additional term */
static void store_term(seq_t val)
{
    ensure_terms_cap(seq_cnt + 1);
    seq_vals[seq_cnt++] = val;
}

/* build a decimal number from an array of decimal digits.
   Works for arbitrary-precision seq_t as well as for native types. */
static seq_t build_number(const uint8_t *digits, int d)
{
    seq_t num = 0;
    for (int i = 0; i < d; ++i) num = num * 10 + digits[i];
    return num;
}

/* count distinct prime substrings of the digit array.
   All operations are performed on seq_t so that the function works
   for both native and arbitrary-precision seq_t. */
static int count_prime_substrings(const uint8_t *digits, int d)
{
    /* maximum number of substrings is d*(d+1)/2 (≤ 28,210 for d≤100) */
    static seq_t primes[8000];
    int uniq = 0;

    for (int i = 0; i < d; ++i) {
        seq_t val = 0;
        for (int j = i; j < d; ++j) {
            val = val * 10 + digits[j];
            if (!is_prime_big(val)) continue;
            bool found = false;
            for (int k = 0; k < uniq; ++k)
                if (primes[k] == val) { found = true; break; }
            if (!found) primes[uniq++] = val;
        }
    }
    return uniq;
}

/* recursively generate the left half of a palindrome.
   When the left half is complete, the full number is built,
   checked for primality and the substring condition,
   and stored if it qualifies.                      */
static void gen_left(int pos, int half, int d, uint8_t *digits)
{
    if (pos == half) {
        /* palindrome constructed – mirror left to right */
        if (d % 2 == 0) {
            for (int i = 0; i < half; ++i) digits[d-1-i] = digits[i];
            seq_t num = build_number(digits, d);
            if (!is_prime_big(num)) return;
            int np = count_prime_substrings(digits, d);
            int need = (3 * d) / 2 + 1;  /* strictly greater than 3*d/2 */
            if (np >= need) store_term(num);
        } else {
            for (uint8_t mid = 0; mid <= 9; ++mid) {
                digits[half] = mid;
                for (int i = 0; i < half; ++i) digits[d-1-i] = digits[i];
                seq_t num = build_number(digits, d);
                if (!is_prime_big(num)) continue;
                int np = count_prime_substrings(digits, d);
                int need = (3 * d) / 2 + 1;
                if (np >= need) store_term(num);
            }
        }
        return;
    }

    if (pos == 0) {
        /* first digit – cannot be even or 5 for d>1 */
        static const uint8_t first[4] = {1,3,7,9};
        for (int t = 0; t < 4; ++t) {
            digits[pos] = first[t];
            gen_left(pos+1, half, d, digits);
        }
    } else {
        for (uint8_t dig = 0; dig <= 9; ++dig) {
            digits[pos] = dig;
            gen_left(pos+1, half, d, digits);
        }
    }
}

/* generate all palindromes of a given length and test them.
   The case d==1 is handled specially to avoid storing
   one‑digit primes that cannot satisfy the substring condition. */
static void generate_palindromes_of_length(int d)
{
    uint8_t digits[64];
    if (d == 1) {
        static const uint8_t singles[4] = {2,3,5,7};
        for (int t = 0; t < 4; ++t) {
            digits[0] = singles[t];
            seq_t num = build_number(digits, d);
            if (!is_prime_big(num)) continue;
            /* need more than 3*d/2 = 1.5 distinct primes – impossible */
            /* Therefore no single‑digit palindrome ever qualifies. */
        }
        return;
    }
    int half = d / 2;
    gen_left(0, half, d, digits);
}

/* generate the sequence up to the requested index */
static void generate_up_to(size_t target)
{
    static int current_d = 1;
    while (seq_cnt < target) {
        generate_palindromes_of_length(current_d);
        ++current_d;
    }
}

/* public function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t target = (size_t)n;
    if (seq_cnt < target) generate_up_to(target);
    return seq_vals[target - 1];
}
