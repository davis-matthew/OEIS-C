/*
 * A182177  Beginning with 0, smallest positive integer not yet in the
 *          sequence such that two adjacent digits of the sequence (also
 *          ignoring commas between terms) sum to a prime.
 * https://oeis.org/A182177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A182177: Beginning with 0, smallest positive integer not yet in the
 * sequence such that two adjacent digits of the sequence (ignoring commas
 * between terms) sum to a prime.
 * The function performs a linear search for the next admissible term,
 * memoising the generated sequence for repeated calls.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* prime_sum[s] == true if s is a prime; otherwise false.  s ranges from 0 to 18. */
static const bool prime_sum[19] = {
    0, /* 0 */
    0, /* 1 */
    1, /* 2 */
    1, /* 3 */
    0, /* 4 */
    1, /* 5 */
    0, /* 6 */
    1, /* 7 */
    0, /* 8 */
    0, /* 9 */
    0, /*10 */
    1, /*11 */
    0, /*12 */
    1, /*13 */
    0, /*14 */
    0, /*15 */
    0, /*16 */
    1, /*17 */
    0  /*18 */
};

/* Check whether the decimal digits of val can be appended to the current
   digit stream ending with last_digit and still satisfy the prime‑sum rule.
   If so, new_last_digit is set to the least‑significant digit of val. */
static bool check_candidate(seq_t val, int last_digit, int *new_last_digit)
{
    /* Maximum number of decimal digits of a 128‑bit integer is < 40. */
    uint8_t buf[40];
    size_t len = 0;
    seq_t x = val;

    /* Special case: val == 0  */
    if (x == (seq_t)0) {
        buf[0] = 0;
        len = 1;
    } else {
        while (x != (seq_t)0) {
            buf[len++] = (uint8_t)(x % (seq_t)10);
            x /= (seq_t)10;
        }
    }

    int first_digit = buf[len - 1];            /* most significant digit of val   */
    if (!prime_sum[last_digit + first_digit]) return false;

    for (int i = (int)len - 1; i > 0; --i) {
        if (!prime_sum[buf[i] + buf[i - 1]]) return false;
    }

    *new_last_digit = buf[0];                  /* least significant digit of val   */
    return true;
}

seq_t a(idx_t n)
/*  Return a(n) for the sequence A182177 (1‑based index).  */
{
    /* Local static state for memoisation.  */
    static seq_t *seq = NULL;          /* array of generated terms       */
    static size_t seq_len = 0;         /* current number of stored terms */
    static size_t seq_cap = 0;         /* allocated capacity             */

    static bool *used = NULL;          /* marks numbers that have already
                                          appeared in the sequence       */
    static size_t used_cap = 0;        /* allocated capacity of used     */

    static int last_digit = 0;         /* last digit of the current stream */

    /* Initialise static variables on first call.  */
    if (seq == NULL) {
        seq_cap = 64;
        seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq_len = 1;
        last_digit = 0;

        used_cap = 32;
        used = (bool *)calloc(used_cap, sizeof(bool));
        used[0] = true;               /* 0 is already in the sequence   */
    }

    /* Ensure that seq has enough capacity.  */
    if (seq_len + 1 > seq_cap) {
        seq_cap *= 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them.  */
    while (seq_len < n) {
        /* Search for the smallest unused candidate.  */
        for (idx_t cand = 1; ; ++cand) {
            /* Grow the used array if necessary.  */
            if ((size_t)cand >= used_cap) {
                size_t new_cap = used_cap ? used_cap * 2 : 32;
                while (new_cap <= (size_t)cand) new_cap <<= 1;
                bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
                for (size_t i = used_cap; i < new_cap; ++i) tmp[i] = false;
                used = tmp;
                used_cap = new_cap;
            }

            if (used[(size_t)cand]) continue;   /* already used */

            seq_t val = (seq_t)cand;
            int new_ldigit;
            if (!check_candidate(val, last_digit, &new_ldigit))
                continue;                      /* prime‑sum condition fails */

            /* Candidate accepted.  Record it.  */
            seq[seq_len++] = val;
            used[(size_t)cand] = true;
            last_digit = new_ldigit;
            break;
        }
    }

    return seq[n - 1];
}
