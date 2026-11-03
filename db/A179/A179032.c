/*
 * A179032  Emirps whose only prime digits are one or more 2's.
 * https://oeis.org/A179032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Emirps whose only prime digits are one or more 2's
 * Range:     u8 none, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 904 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An emirp is a non‑palindromic prime whose reversal is also prime.
   This function generates all such emirps that consist only of the
   decimal digits 0,1,2,4,6,8,9 (i.e. no prime digit other than 2).
   The terms are produced in ascending numeric order.  The routine
   memoises results so that subsequent calls in any order are O(1). */

#include <stdbool.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"
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

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Check whether all digits of num are from the allowed set
   and whether at least one digit 2 appears.  The result is
   stored in *has2.  Returns true only if the whole number
   consists of allowed digits and *has2 == true.  */
static bool passes_allowed_and_has2(idx_t num, bool *has2)
{
    static const bool allowed[10] =
        { true,  true,  true,  false, true,  false, true,  false, true,  true }; /* 0,1,2,4,6,8,9 */

    *has2 = false;
    if (num == 0) return false;

    idx_t t = num;
    while (t > 0) {
        int d = (int)(t % 10);
        if (d == 2) *has2 = true;
        if (!allowed[d]) return false;
        t /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    static seq_t *terms   = NULL; /* array of found emirps */
    static size_t terms_cap = 0;   /* current capacity of terms */
    static idx_t terms_cnt  = 0;   /* number of filled entries */
    static idx_t last_checked = 2; /* last integer examined */

    /* initial allocation */
    if (!terms) {
        terms_cap = 64;
        terms = seq_alloc_array(terms_cap);
        terms_cnt = 0;
    }

    /* generate until we have at least n terms */
    if (n > terms_cnt) {
        bool has2;
        while (terms_cnt < n) {
            /* find next candidate that has only allowed digits
               and contains at least one 2 */
            while (true) {
                if (passes_allowed_and_has2(last_checked, &has2) && has2)
                    break;
                last_checked++;
            }

            seq_t cur = (seq_t)last_checked;

            /* primality tests */
            if (is_prime_big(cur)) {
                seq_t rev = reverse_digits(cur);
                if (rev != cur && is_prime_big(rev)) {
                    /* store the emirp */
                    if (terms_cnt == terms_cap) {
                        grow_seq_array(&terms, &terms_cap, terms_cap * 2);
                    }
                    terms[terms_cnt++] = cur;
                }
            }
            last_checked++; /* move on to next integer */
        }
    }

    return terms[n - 1];
}
