/*
 * A034996  Related to octo-factorial numbers A045755.
 * https://oeis.org/A034996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^(n-1)*A045755(n)/n! where * A045755(n) =
 *            Product_{j=1..n} (8*j-7) * Recurrence: a(1)=1,
 *            a(n)=a(n-1)*8*(8*n-7)/n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;

#ifdef __cplusplus
    /* In C++ builds (including arbitrary‑precision types) we can
       rely on operator overloading for multiplication and division. */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t mul = (seq_t)8 * (seq_t)(8 * i - 7);
        result *= mul;
        result /= (seq_t)i;
    }
#else
    /* In pure C builds (uint64_t etc.) use a 128‑bit temporary to
       avoid overflow in intermediate products.  The final
       division guarantees an exact result.  The compiler must
       support __uint128_t. */
#   if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    for (idx_t i = 2; i <= n; ++i) {
        __uint128_t temp = (__uint128_t)result * 8;
        temp *= (8 * i - 7);
        temp /= i;
        result = (seq_t)temp;
    }
#   else
    /* Fallback for compilers without native 128‑bit integer support.
       The multiplication order is chosen to keep intermediate values
       as small as possible; for n values where the final result still
       fits in 'seq_t', the intermediate will not overflow in 64‑bit
       builds. */
    for (idx_t i = 2; i <= n; ++i) {
        result *= (seq_t)8;
        result *= (seq_t)(8 * i - 7);
        result /= (seq_t)i;
    }
#   endif
#endif

    return result;
}
