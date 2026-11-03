/*
 * A235168  Triangle read by rows: row n gives digits of n in primorial base.
 * https://oeis.org/A235168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base, look
 * Formula:   not stated
 * Range:     u8 n <= 12447, u16 n <= 12447, u32 n <= 12447, u64 n <= 12447,
 *            u128 n <= 12447, big any n
 * Validated: standard, pass, 102 terms through n = 9912 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A235168: digits of non‑negative integers written in
   primorial base, read row by row.  The function returns the
   nth digit in this flattened table. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static idx_t primorial_len(idx_t x)
{
    if (x == 0) return 1;            // base case: row 0 has one digit 0
    idx_t len = 0;
    idx_t prime_index = 1;           // get_prime(k) uses 1‑based indexing
    while (x > 0) {
        x /= get_prime(prime_index);
        prime_index++;
        len++;
    }
    return len;
}

static seq_t primorial_digit(idx_t x, idx_t pos)
{
    if (x == 0) return 0;            // digit of row 0
    seq_t digits[64];                // enough for 64‑bit numbers
    idx_t len = 0;
    idx_t prime_index = 1;
    while (x > 0) {
        digits[len++] = x % get_prime(prime_index);
        x /= get_prime(prime_index);
        prime_index++;
    }
    /* digits[0] is least significant, digits[len‑1] most significant */
    return digits[len - 1 - pos];
}

seq_t a(idx_t n)
{
    idx_t total = 0;          // cumulative count of digits so far
    idx_t row = 0;
    while (1) {
        idx_t len = primorial_len(row);
        if (total + len > n) break;
        total += len;
        row++;
    }
    idx_t pos = n - total;    // position within the current row (0 = msb)
    return primorial_digit(row, pos);
}
