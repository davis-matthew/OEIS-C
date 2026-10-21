/*
 * A063559  Smallest k such that 3^k has exactly n 4's in its decimal
 *          representation.
 * https://oeis.org/A063559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A063559: a(n) = smallest k such that 3^k has exactly n fours
   in its decimal representation.  The routine scans successive powers of 3,
   keeps a big‑integer in base 10⁹, and updates the number of digit 4s
   incrementally.  Results are memoised and can be queried in any order. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Digit‑count tables ---------- */
static uint8_t count4_5[100000];       /* 0 … 99999 : how many ‘4’ in the 5‑digit
                                         representation (with leading zeros)   */
static bool count4_5_inited = false;

static void init_count4_5(void)
{
    if (count4_5_inited) return;
    for (int i = 0; i < 100000; ++i) {
        int v = i, c = 0;
        for (int j = 0; j < 5; ++j) {
            if (v % 10 == 4) ++c;
            v /= 10;
        }
        count4_5[i] = (uint8_t)c;
    }
    count4_5_inited = true;
}

/* ---------- Big integer in base 10⁹ ---------- */
#define BASE 1000000000U          /* 10⁹, fits in 32 bits */
static uint32_t *digits = NULL;    /* little‑endian limbs */
static size_t digits_len = 0;      /* number of used limbs */
static size_t digits_cap = 0;      /* allocated limbs */

/* Ensure capacity for at least “needed” limbs. */
static void ensure_digits_cap(size_t needed)
{
    if (needed <= digits_cap) return;
    size_t new_cap = digits_cap ? digits_cap * 2 : 4;
    while (new_cap < needed) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(digits, new_cap * sizeof(uint32_t));
    if (!tmp) abort();
    digits = tmp;
    digits_cap = new_cap;
}

/* ---------- Memoised sequence storage ---------- */
static seq_t *seq_vals = NULL;     /* seq_vals[n] = first k with n fours */
static size_t seq_cap = 0;         /* allocated entries */

/* Ensure capacity for at least “needed” seq entries. */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap = new_cap;
    /* initialise new slots to 0 */
    for (size_t i = seq_cap; i < new_cap; ++i) seq_vals[i] = 0;
}

/* ---------- Main routine ---------- */
seq_t a(idx_t n)
{
    init_count4_5();

    /* Initialise big integer and memo table on first call. */
    if (!digits) {
        ensure_digits_cap(1);
        digits_len = 1;
        digits[0] = 3;                     /* 3¹ */
        ensure_seq_cap(1);
        seq_vals[0] = (seq_t)1;            /* a(0) = 1 */
    }

    /* Total number of digit 4s in the current power. */
    static size_t total_fours = 0;
    static idx_t current_k = 1;          /* already at 3¹ */

    /* For the first call after initialisation, count the fours in 3¹. */
    if (total_fours == 0 && seq_vals[0] == 1 && current_k == 1) {
        /* 3 has no fours; total_fours stays 0. */
    }

    /* Generate further powers until we know a(n). */
    while (n > 0 && (n >= seq_cap || seq_vals[n] == 0)) {

        /* Multiply the big integer by 3, updating total_fours. */
        uint64_t carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            uint32_t old = digits[i];
            uint64_t prod = (uint64_t)old * 3 + carry;
            uint32_t newv = (uint32_t)(prod % BASE);
            carry = prod / BASE;

            /* Count transition of this limb. */
            uint32_t old_low  = old % 100000U;
            uint32_t old_high = old / 100000U;
            uint32_t new_low  = newv % 100000U;
            uint32_t new_high = newv / 100000U;
            total_fours +=
                (size_t)count4_5[new_low] + (size_t)count4_5[new_high]
                - (size_t)count4_5[old_low] - (size_t)count4_5[old_high];

            digits[i] = newv;
        }

        /* Add new limbs for any remaining carry. */
        while (carry) {
            uint32_t newv = (uint32_t)(carry % BASE);
            carry /= BASE;
            ensure_digits_cap(digits_len + 1);
            digits[digits_len++] = newv;

            uint32_t low = newv % 100000U;
            uint32_t high = newv / 100000U;
            total_fours += (size_t)count4_5[low] + (size_t)count4_5[high];
        }

        /* Advance to the next exponent. */
        ++current_k;

        /* Record the first occurrence of this number of fours. */
        if (total_fours >= seq_cap) {
            /* Extend the seq array sufficiently. */
            ensure_seq_cap(total_fours + 1);
        }
        if (seq_vals[total_fours] == 0) {
            seq_vals[total_fours] = (seq_t)current_k;
        }
    }

    return seq_vals[n];
}
