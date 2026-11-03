/*
 * A093860  Begin with the n-th composite number (A002808(n)) and go on adding
 *          successive composite numbers; a(n) is the first prime that arises
 *          as the sum.
 * https://oeis.org/A093860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A093860:  a(n) is the first prime obtained as the sum of successive
   composite numbers starting with the n‑th composite number.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Global storage for composite numbers, 1‑based index. */
static seq_t *composites = NULL;   /* array of composites */
static size_t comp_cap   = 0;      /* allocated capacity */
static size_t comp_size  = 0;      /* number of composites stored */
static idx_t  next_test  = 4;      /* next integer to test for compositeness */

/* Ensure composites[0 .. idx-1] are filled (i.e., first idx composites). */
static void ensure_composite_index(idx_t idx)
{
    while (comp_size < idx) {
        if (next_test >= 4 && !is_prime_seq((uint64_t)next_test)) {
            if (comp_size == comp_cap) {
                size_t new_cap = comp_cap ? comp_cap * 2 : 16;
                seq_t *tmp = seq_realloc(composites, comp_cap, new_cap);
                if (!tmp) abort();  /* unrecoverable allocation failure */
                composites = tmp;
                comp_cap = new_cap;
            }
            composites[comp_size++] = (seq_t)next_test;
        }
        ++next_test;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; behaviour below is unused */

    /* Obtain the n‑th composite number. */
    ensure_composite_index(n);
    seq_t sum = composites[n - 1];

    /* Accumulate successive composites until a prime sum is found. */
    idx_t j = n;                     /* next composite index to add (1‑based) */
    while (!is_prime_big(sum)) {
        ++j;
        ensure_composite_index(j);
        sum += composites[j - 1];
    }
    return sum;
}
