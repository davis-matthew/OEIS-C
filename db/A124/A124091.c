/*
 * A124091  Decimal expansion of Fibonacci binary constant: Sum{i>=0}
 *          (1/2)^Fibonacci(i).
 * https://oeis.org/A124091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = decimal digit of the constant Σ 2^{-Fibonacci(i)}
 * Range:     u8 none, u16 n <= 9891, u32 n <= 9891, u64 n <= 9891,
 *            u128 n <= 9891, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset = 1 (a(1) = the integer part) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Number of decimal digits the pre‑computed constant must support.
   For 10 000 decimal digits, at least 33220 binary bits are needed.
   (33220 * log10(2) ≈ 10000)                                      */
#define M_BITS   33224      /* small safety margin */

/* Helper constants for 32‑bit words. */
#define WORD_BITS 32
#define WORD_MASK 0xffffffffU

/* Number of 32‑bit words required to store M_BITS bits (+2 for carries). */
#define WORDS ((M_BITS + 31) / WORD_BITS + 3)

/* Global buffers. */
static uint32_t N[WORDS];          /* numerator bits of the sum   */
static size_t   N_len;             /* used word count of N        */

static uint32_t R[WORDS];          /* current remainder           */
static size_t   R_len;             /* used word count of R        */

static seq_t    integer_part;      /* the integer part (a(1))     */
static unsigned char digits[200001];/* cached decimal digits        */
static size_t   digits_len = 0;     /* digits generated so far      */

static idx_t    fib[100];          /* Fibonacci numbers up to M   */
static size_t   fibcnt;            /* number of Fibonacci values  */

static bool initialized = false;   /* lazy initialisation flag    */

/* ------------------------------------------------------------------ */
/* Add a single bit 1 at position k into an array of 32‑bit words.   */
/* k = 0 refers to the least significant bit.                       */
/* ------------------------------------------------------------------ */
static void add_bit(uint32_t *num, size_t *len, size_t k)
{
    size_t w = k / WORD_BITS;
    unsigned b = (unsigned)(k % WORD_BITS);
    uint64_t sum = (uint64_t)num[w] + ((uint64_t)1U << b);
    num[w] = (uint32_t)(sum & WORD_MASK);
    uint64_t carry = sum >> WORD_BITS;
    size_t i = w + 1;
    while (carry) {
        uint64_t t = (uint64_t)num[i] + carry;
        num[i] = (uint32_t)(t & WORD_MASK);
        carry = t >> WORD_BITS;
        ++i;
    }
    if (i > *len) *len = i;
}

/* ------------------------------------------------------------------ */
/* Compute all Fibonacci numbers that are needed (≤ M_BITS).          */
/* ------------------------------------------------------------------ */
static void compute_fib(void)
{
    fib[0] = 0;
    fib[1] = 1;
    fibcnt = 2;
    while (fib[fibcnt-1] <= (idx_t)M_BITS) {
        idx_t next = fib[fibcnt-1] + fib[fibcnt-2];
        fib[fibcnt++] = next;
        if (fibcnt >= sizeof(fib)/sizeof(fib[0])) break; /* safety */
    }
}

/* ------------------------------------------------------------------ */
/* Shift a big integer (array of 32‑bit words) right by s bits.        */
/* The result is returned as seq_t (fits into the internal type).     */
/* ------------------------------------------------------------------ */
static seq_t shift_right(const uint32_t *num, size_t len, idx_t s)
{
    size_t word_shift = (size_t)(s / WORD_BITS);
    int  bit_shift   = (int)(s % WORD_BITS);
    if (word_shift >= len) return (seq_t)0;

    if (bit_shift == 0) {
        return (seq_t)num[word_shift];
    } else {
        seq_t low  = (seq_t)num[word_shift] >> bit_shift;
        seq_t high = 0;
        if (word_shift + 1 < len)
            high = (seq_t)num[word_shift + 1] << (WORD_BITS - bit_shift);
        return low | high;
    }
}

/* ------------------------------------------------------------------ */
/* Initialise all static data.                                         */
/* ------------------------------------------------------------------ */
static void init(void)
{
    compute_fib();

    /* Build the numerator N = Σ 2^{M_BITS - F_i} */
    for (size_t i = 0; i < WORDS; ++i) N[i] = 0;
    N_len = 0;
    for (size_t i = 0; i < fibcnt; ++i) {
        if (fib[i] > (idx_t)M_BITS) break;
        add_bit(N, &N_len, M_BITS - (size_t)fib[i]);
    }

    /* Determine the integer part */
    integer_part = shift_right(N, N_len, M_BITS);

    /* Initialise the remainder R = N mod 2^{M_BITS} */
    size_t rem_words = M_BITS / WORD_BITS + 1;
    for (size_t i = 0; i < rem_words; ++i) R[i] = N[i];
    R_len = rem_words;
    size_t last_word = rem_words - 1;
    uint32_t mask;
    if ((M_BITS % WORD_BITS) == 0)
        mask = WORD_MASK;
    else
        mask = (1U << (M_BITS % WORD_BITS)) - 1;
    R[last_word] &= mask;

    digits_len = 0;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Multiply the current remainder R by 10 (big integer * 10).          */
/* ------------------------------------------------------------------ */
static void mul_rem_by10(void)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < R_len; ++i) {
        uint64_t prod = (uint64_t)R[i] * 10 + carry;
        R[i] = (uint32_t)(prod & WORD_MASK);
        carry = prod >> WORD_BITS;
    }
    if (carry) {
        if (R_len < WORDS)
            R[R_len++] = (uint32_t)carry;
        else
            R[R_len - 1] = (uint32_t)carry;   /* safety fall‑back */
    }
}

/* ------------------------------------------------------------------ */
/* Mask the remainder R to keep only the lowest M_BITS bits (mod 2^M). */
/* ------------------------------------------------------------------ */
static void mask_remainder(void)
{
    size_t last_word = M_BITS / WORD_BITS;
    uint32_t mask;
    if ((M_BITS % WORD_BITS) == 0)
        mask = WORD_MASK;
    else
        mask = (1U << (M_BITS % WORD_BITS)) - 1;
    R[last_word] &= mask;
    for (size_t i = last_word + 1; i < R_len; ++i)
        R[i] = 0;
    R_len = last_word + 1;
}

/* ------------------------------------------------------------------ */
/* Generate decimal digits up to position `pos` (0‑based after the
   integer part).                                                     */
/* ------------------------------------------------------------------ */
static void generate_to(size_t pos)
{
    while (digits_len <= pos) {
        mul_rem_by10();                     /* R := R * 10 */
        seq_t d = shift_right(R, R_len, M_BITS);
        digits[digits_len++] = (unsigned char)d;
        mask_remainder();                   /* keep lower M_BITS bits */
    }
}

/* ------------------------------------------------------------------ */
/* The required sequence function.                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init();

    if (n == 1) return integer_part;

    size_t pos = (size_t)(n - 2);           /* 0‑based decimal position */
    if (pos >= 200001)   /* safety guard for extremely large requests */
        generate_to(pos);
    else if (digits_len <= pos)
        generate_to(pos);

    return (seq_t)digits[pos];
}
