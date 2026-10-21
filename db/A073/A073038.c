/*
 * A073038  Take A000040, omit commas: 23571113171923..., select 5-digit
 *          primes seen when scanning from left.
 * https://oeis.org/A073038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation:  Concatenate the primes of A000040, slide a 5‑digit window
    over the resulting digit stream, and output the value of each window
    that itself is a prime (the window may start with zeros).  All terms
    are stored in a static array and the function returns the n‑th term
    for any n ≥ 1, independent of call order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*  Helper:  sieve of Eratosthenes up to 100 000 to test 5‑digit windows  */
static bool window_prime[100000] = {0};

static void init_window_primes(void)
{
    /* Treat 0 and 1 as composite.  All other indices are initially prime. */
    for (int i = 2; i < 100000; ++i)
        window_prime[i] = true;

    for (int i = 2; i * i < 100000; ++i)
        if (window_prime[i])
            for (int j = i * i; j < 100000; j += i)
                window_prime[j] = false;
}

/*  Helper:  incremental prime generator using trial division  */
static uint64_t   prime_candidate = 3;          /* next odd number to test   */
static uint64_t  *prime_list      = NULL;      /* dynamic list of primes   */
static size_t     prime_cnt       = 0;          /* number of primes stored  */
static size_t     prime_cap       = 0;          /* current capacity         */

/*  Append a prime to the list, expanding the array if necessary  */
static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!prime_list) exit(1);
    }
    prime_list[prime_cnt++] = p;
}

/*  Return the next prime > previous one  */
static uint64_t next_prime(void)
{
    if (prime_cnt == 0) {        /* first prime */
        add_prime(2);
        prime_candidate = 3;
        return 2;
    }

    for (;;) {
        uint64_t n = prime_candidate;

        /* test divisibility by existing primes up to sqrt(n) */
        bool is_p = true;
        uint64_t lim = 1;
        while (lim <= n / lim) ++lim;   /* lim = floor(sqrt(n)) + 1 */
        --lim;
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= lim; ++i)
            if (n % prime_list[i] == 0) { is_p = false; break; }

        if (is_p) {
            add_prime(n);
            uint64_t res = n;
            prime_candidate += 2;        /* skip even numbers */
            return res;
        }
        prime_candidate += 2;
    }
}

/*  Storage for generated sequence terms  */
static seq_t *seq_terms   = NULL;
static size_t seq_len      = 0;
static size_t seq_cap      = 0;

/*  Sliding window over the digit stream  */
static unsigned win_len  = 0;   /* number of digits currently in window  */
static unsigned win_val  = 0;   /* integer value of the last five digits   */

/*  One‑time initialization flag  */
static bool init_done = false;

/*  Main sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    if (!init_done) {
        init_window_primes();

        seq_cap    = 64;
        seq_terms  = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        if (!seq_terms) exit(1);

        init_done = true;
    }

    /* generate terms until we have at least n of them  */
    while (seq_len < (size_t)n) {
        uint64_t p = next_prime();

        /* convert prime to digits from most significant to least */
        unsigned digits[20];
        int dlen = 0;
        do {
            digits[dlen++] = (unsigned)(p % 10);
            p /= 10;
        } while (p > 0);

        for (int k = dlen - 1; k >= 0; --k) {
            unsigned digit = digits[k];

            win_val = win_val * 10 + digit;
            if (win_len < 5)
                ++win_len;
            else
                win_val %= 100000;   /* keep only last 5 digits */

            if (win_len == 5 && window_prime[win_val]) {
                /* store the 5‑digit prime value */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 64;
                    seq_terms = (seq_t*)realloc(seq_terms,
                                                seq_cap * sizeof(seq_t));
                    if (!seq_terms) exit(1);
                }
                seq_terms[seq_len++] = (seq_t)win_val;
            }
        }
    }

    return seq_terms[n - 1];
}
