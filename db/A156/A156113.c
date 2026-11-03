/*
 * A156113  Seven-digit primes that begin with 1 followed by permutations of
 *          digits 2 to 7.
 * https://oeis.org/A156113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: full, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the n-th 7‑digit prime that begins with 1 and
   has the remaining digits a permutation of 2,3,4,5,6,7.
   The sequence is finite; a(n) = 0 for n > 63. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* Static array to hold all prime candidates (max 6! = 720).  */
static seq_t primes_arr[720];
static size_t prime_cnt = 0;
static bool initialized = false;

/* Comparator for qsort. */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb);
}

/* Recursive generation of permutations of digits 2–7. */
static void permute(int depth, uint8_t used_mask, seq_t cur)
{
    if (depth == 6) {
        if (is_prime_big(cur))
            primes_arr[prime_cnt++] = cur;
        return;
    }
    static const uint8_t digits[6] = {2, 3, 4, 5, 6, 7};
    for (int i = 0; i < 6; ++i) {
        if (!(used_mask & (1 << i))) {
            permute(depth + 1, used_mask | (1 << i), cur * 10 + digits[i]);
        }
    }
}

/* Initialises the prime list on first use. */
static void init_primes(void)
{
    if (initialized) return;
    prime_cnt = 0;
    permute(0, 0, 1);               /* start with the leading '1' */
    mem_qsort(primes_arr, prime_cnt, sizeof(seq_t), cmp_seq);   /* (raw qsort must not move seq_t under big) */
    initialized = true;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    init_primes();
    if (n == 0) return 0;          /* offset is 1 */
    idx_t idx = n - 1;
    if (idx < prime_cnt) return primes_arr[idx];
    return 0;                      /* finite sequence */
}
