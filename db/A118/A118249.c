/*
 * A118249  a(0)=0. Concatenate onto the end of the sequence (from left to
 *          right) the integer m_n converted into binary and reversed (with
 *          the most significant digit on the right), where m_n is the
 *          smallest integer > A118250(n-1) and whose reversed binary
 *          representation does not occur anywhere earlier in the...
 * https://oeis.org/A118249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5167, u16 n <= 5167, u32 n <= 5167, u64 n <= 5167,
 *            u128 n <= 5167, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth bit of the infinite binary sequence obtained by
   repeatedly appending, for each integer m, the bitwise reverse of the
   binary representation of m, where m is the smallest integer greater than the
   previous chosen m whose reversed binary does not occur earlier in the
   sequence.  Initial term a(0)=0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static data that persists between calls. */
    static uint8_t *bits = NULL;      /* concatenated bit sequence */
    static size_t bits_len = 0;       /* current length of bits array   */
    static size_t bits_cap = 0;       /* allocated capacity             */
    static uint64_t last_m = 0;       /* last chosen integer m          */
    static bool initialized = false; /* have we performed initialization? */

    /* One-time initialization.  a(0) is defined as 0 independently of the
       algorithm, so we start the sequence with that bit. */
    if (!initialized) {
        bits_cap = 8;
        bits = (uint8_t *)malloc(bits_cap * sizeof(*bits));
        if (!bits) exit(EXIT_FAILURE); /* minimal error handling */
        bits[0] = 0;
        bits_len = 1;
        initialized = true;
    }

    /* Generate bits until we have at least n+1 bits (indices 0..n). */
    while (bits_len <= n) {
        /* Find smallest integer m > last_m whose reversed binary
           does not occur earlier in the sequence. */
        uint64_t candidate = last_m + 1;
        while (true) {
            /* Obtain reversed binary representation of candidate
               into a temporary array. MSB becomes last element. */
            unsigned int revbits[64];
            size_t revlen = 0;
            uint64_t tmp = candidate;
            while (tmp > 0) {
                revbits[revlen++] = (unsigned int)(tmp & 1U);
                tmp >>= 1U;
            }
            /* Edge case: candidate == 0 never occurs (candidate > 0). */

            /* Search for this pattern in the already existing bits. */
            bool found = false;
            if (revlen <= bits_len) {
                for (size_t start = 0; start + revlen <= bits_len; ++start) {
                    bool match = true;
                    for (size_t k = 0; k < revlen; ++k) {
                        if (bits[start + k] != (uint8_t)revbits[k]) {
                            match = false;
                            break;
                        }
                    }
                    if (match) {
                        found = true;
                        break;
                    }
                }
            }
            /* If pattern not found, accept this candidate. */
            if (!found) {
                /* Ensure capacity for new bits. */
                if (bits_len + revlen > bits_cap) {
                    size_t new_cap = bits_cap * 2;
                    if (new_cap < bits_len + revlen) new_cap = bits_len + revlen;
                    uint8_t *tmp_ptr = (uint8_t *)realloc(bits, new_cap);
                    if (!tmp_ptr) exit(EXIT_FAILURE);
                    bits = tmp_ptr;
                    bits_cap = new_cap;
                }
                /* Append reversed bits to the sequence. */
                for (size_t k = 0; k < revlen; ++k)
                    bits[bits_len++] = revbits[k];
                /* Update last chosen integer. */
                last_m = candidate;
                break;  /* finish this iteration of while(bits_len <= n) */
            }
            /* Otherwise, try next candidate. */
            ++candidate;
        }
    }

    /* Return the requested bit as seq_t. */
    return (seq_t)bits[n];
}
