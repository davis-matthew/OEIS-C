/*
 * A052016  Primes with digits in descending order that differ exactly by 1.
 * https://oeis.org/A052016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   primes whose decimal digits form a consecutive descending run
 *            (e.g., 43, 76543, …). The sequence is obtained by generating all
 *            numbers with such digit patterns, testing each for primality,
 *            and sorting the primes in ascending order
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: full, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
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

/* Comparator for qsort on seq_t values. */
static int cmp_primes(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

static seq_t primes[54];               /* maximum possible candidates */
static size_t primes_count = 0;
static bool primes_prepared = false;

/* Builds the list of all primes that satisfy the digit condition. */
static void prepare_primes(void)
{
    if (primes_prepared)
        return;

    char buf[12];          /* enough for 10 digits + null */
    for (uint8_t start = 1; start <= 9; ++start) {
        for (uint8_t len = 1; len <= start + 1; ++len) {
            size_t pos = 0;
            for (uint8_t i = 0; i < len; ++i) {
                buf[pos++] = '0' + (start - i);
            }
            buf[pos] = '\0';
            seq_t num = seq_from_str(buf);
            if (is_prime_big(num)) {
                primes[primes_count++] = num;
            }
        }
    }

    /* Sort the primes in ascending order. */
    if (primes_count > 1) {
        mem_qsort(primes, primes_count, sizeof(seq_t), cmp_primes);
    }

    primes_prepared = true;
}

seq_t a(idx_t n)
{
    prepare_primes();

    if (n == 0 || (size_t)n > primes_count)
        return (seq_t)0;

    return primes[(size_t)n - 1];
}
