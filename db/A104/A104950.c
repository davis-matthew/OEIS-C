/*
 * A104950  Primes from merging of 7 successive digits in decimal expansion of
 *          the Champernowne Constant.
 * https://oeis.org/A104950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = primes obtained by merging every 7 successive decimal
 *            digits of the Champernowne constant, ignoring windows that start
 *            with digit 0
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*---  Prime sieve up to 10⁷ ------------------------------------------- */
    static uint8_t *prime_sieve = NULL;
    static bool sieve_done = false;
    if (!sieve_done) {
        const uint32_t LIM = 10000000u;          /* 10^7  → indices 0 .. 9,999,999   */
        prime_sieve = (uint8_t *)malloc(LIM * sizeof(uint8_t));
        if (!prime_sieve) return (seq_t)0;       /* allocation failed – return 0   */
        for (uint32_t i = 0; i < LIM; ++i) prime_sieve[i] = 1;
        prime_sieve[0] = 0;
        prime_sieve[1] = 0;
        uint32_t limit = 3162;                  /* floor(sqrt(10^7))             */
        for (uint32_t i = 2; i <= limit; ++i) {
            if (prime_sieve[i]) {
                for (uint32_t j = i * i; j < LIM; j += i) prime_sieve[j] = 0;
            }
        }
        sieve_done = true;
    }

    /*---  Sliding 7‑digit window over Champernowne digits ----------------- */
    uint32_t cur_window = 0;            /* integer value of current window      */
    uint8_t window[7] = {0};            /* ring buffer holding the 7 digits      */
    uint8_t window_start = 0;           /* index of the oldest digit in window  */
    uint8_t digits_seen = 0;            /* count of digits collected so far      */
    uint64_t found = 0;                 /* number of primes found so far         */
    uint64_t num = 1;                   /* current integer to emit digits from   */

    /* constants */
    const uint32_t WINDOW_SIZE = 7u;
    const uint32_t WINDOW_MOD  = 10000000u;  /* 10^7                               */

    while (found < n) {
        /* extract digits of num (least significant digit first) */
        uint8_t temp_digits[20];
        int tcnt = 0;
        uint64_t t = num;
        do {
            temp_digits[tcnt++] = t % 10;
            t /= 10;
        } while (t > 0);

        /* process digits from most significant to least */
        for (int k = tcnt - 1; k >= 0; --k) {
            uint8_t d = temp_digits[k];

            if (digits_seen < WINDOW_SIZE) {
                /* fill the window until it is full */
                window[digits_seen] = d;
                ++digits_seen;
                if (digits_seen == WINDOW_SIZE) {
                    /* compute initial window value */
                    cur_window = 0;
                    for (uint8_t i = 0; i < WINDOW_SIZE; ++i)
                        cur_window = cur_window * 10 + window[i];
                    /* check for prime only if first digit ≠ 0 */
                    if (window[0] != 0 && prime_sieve[cur_window]) {
                        ++found;
                        if (found == n) return (seq_t)cur_window;
                    }
                }
            } else {
                /* window already full – slide it */
                cur_window = (cur_window * 10 + d) % WINDOW_MOD;
                /* overwrite oldest digit in the ring buffer */
                window[window_start] = d;
                window_start = (window_start + 1) % WINDOW_SIZE;
                /* first digit of the new window is window[window_start] */
                if (window[window_start] != 0 && prime_sieve[cur_window]) {
                    ++found;
                    if (found == n) return (seq_t)cur_window;
                }
            }
        }
        ++num;
    }
    return (seq_t)0;   /* Control will never reach here for n >= 1 */
}
