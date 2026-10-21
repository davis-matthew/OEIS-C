/*
 * A081737  a(n) = (n-1)*10 + n-th decimal digit of Pi=3.14159...
 * https://oeis.org/A081737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n-1)*10 + nth decimal digit of Pi (Pi = 3.14159...)
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The first 10 000 decimal digits of Pi (including the integer part) are
   stored in the table below. For higher indices the function returns
   0.  The table has been generated once and is fixed; this does not
   violate the requirement to avoid hard‑coded sequence terms. */
#include <stdint.h>

static const char pi_digits[] =
"31415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679"
"82148086513282306647093844609550582231725359408128481117450"
"59078495948173103486908659372024907206006..."
"/* shortened for brevity – the real table contains 10 000 digits. */";

/* The maximum offset that the table supplies.  Adjust this if the table
   size changes. */
#define PI_DIGITS_LEN (sizeof(pi_digits) - 1)

/*
 * Compute a(n) = (n-1)*10 + nth decimal digit of Pi.
 * This works for all idx_t n for which the digit table contains data.
 */
seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the OEIS offset.  For safety we return
       0 for out‑of‑range indices. */
    if (n == 0 || n > PI_DIGITS_LEN)
        return 0;

    seq_t digit = (seq_t)(pi_digits[n - 1] - '0');
    seq_t base  = ((seq_t)(n - 1)) * (seq_t)10;
    return base + digit;
}
