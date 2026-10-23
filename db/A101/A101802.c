/*
 * A101802  Primes which remain prime after operation "omit leftmost maximal
 *          digit" repeated until only one digit remains.
 * https://oeis.org/A101802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime_seq, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A101802 – Primes whose successive “omit leftmost maximal digit” reductions
 * all remain prime, with the least digit a single prime digit.  
 * The function performs a search over consecutive integers, starting at 10,
 * testing each prime for the property.  This yields the sequence in the
 * correct order (offset 1).  All operations use only integer arithmetic
 * allowed by the specification. */
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/seq_to_dec_string.h"

static bool is_valid_prime(seq_t p) {
    char buf[512];
    seq_to_dec_string(p, buf, sizeof(buf));

    /* Step 1 – check the least digit condition */
    int min_digit = 10;
    int min_count = 0;
    for (int i = 0; buf[i]; ++i) {
        int d = buf[i] - '0';
        if (d < min_digit) {
            min_digit = d;
            min_count = 1;
        } else if (d == min_digit) {
            ++min_count;
        }
    }
    if (!(min_digit == 2 || min_digit == 3 || min_digit == 5 || min_digit == 7))
        return false;
    if (min_count != 1)
        return false;

    /* Step 2 – iterative removal of the leftmost maximal digit */
    seq_t cur = p;
    while (true) {
        seq_to_dec_string(cur, buf, sizeof(buf));
        int len = 0;
        while (buf[len]) ++len;
        if (len == 1) break;          /* reached a single‑digit prime */

        /* locate leftmost maximal digit */
        int max_digit = -1;
        int max_idx   = 0;
        for (int i = 0; i < len; ++i) {
            int d = buf[i] - '0';
            if (d > max_digit) {
                max_digit = d;
                max_idx   = i;
            }
        }

        /* build the next number by omitting that digit */
        seq_t next = 0;
        for (int i = 0; i < len; ++i) {
            if (i == max_idx) continue;
            next = next * 10 + (buf[i] - '0');
        }

        if (!is_prime_seq(next))
            return false;
        cur = next;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                   /* undefined for offset 1 */

    idx_t found = 0;
    seq_t cand  = 10;               /* start at first two‑digit integer */

    while (true) {
        if (is_prime_seq(cand)) {
            if (is_valid_prime(cand)) {
                ++found;
                if (found == n)
                    return cand;
            }
        }
        cand++;
    }
}
